/*
 * Copyright 2022 Morse Micro
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "morse.h"

#include <stdarg.h>

#define MORSE_CLI_CMDLINE_MAXLEN	(256)

#define MORSE_CLI_COMMAND "morse_cli"

int morse_cli(const char *ifname, const char *args_fmt, ...)
{
	int ret;
	char morse_cli_cmdline[MORSE_CLI_CMDLINE_MAXLEN];
	va_list vargs;
	char *buf;
	size_t spc;

	buf = morse_cli_cmdline;
	spc = sizeof(morse_cli_cmdline);

	if (ifname != NULL)
		ret = snprintf(buf, spc, "%s -i %s ", MORSE_CLI_COMMAND, ifname);
	else
		ret = snprintf(buf, spc, "%s ", MORSE_CLI_COMMAND);

	if (ret < 0)
		return ret;

	if ((unsigned int)ret >= sizeof(morse_cli_cmdline))
		return -1; /* Buffer too small */

	buf += (unsigned int)ret;
	spc -= (unsigned int)ret;

	va_start(vargs, args_fmt);
	ret = vsnprintf(buf, spc, args_fmt, vargs);
	va_end(vargs);

	if (ret < 0)
		return ret;

	if ((unsigned int)ret >= sizeof(morse_cli_cmdline))
		return -1; /* Buffer too small */

	wpa_printf(MSG_DEBUG, "morse: execute %s", morse_cli_cmdline);

	ret = system(morse_cli_cmdline);

	return WEXITSTATUS(ret);
}

int morse_set_long_sleep_enabled(const char *ifname, bool enabled)
{
	const char *operation = enabled ? "enable" : "disable";
	int ret;

	wpa_printf(MSG_INFO, "morse: %s long sleep on ifname %s", operation, ifname);

	ret = morse_cli(ifname, "long_sleep %s", operation);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to %s long sleep on ifname %s",
			operation, ifname);

	return ret;
}

int morse_set_s1g_op_class(const char *ifname, u8 opclass, u8 prim_opclass)
{
	int ret;

	ret = morse_cli(ifname, "opclass %u -l %u", opclass, prim_opclass);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set s1g op class on ifname %s",
			ifname);

	return ret;
}

int morse_set_channel(const char *ifname, int oper_freq, int oper_chwidth, u8 prim_chwidth,
			u8 prim_1mhz_ch_idx)
{
	int ret;

	ret = morse_cli(ifname, "channel -n %d -c %d -o %d -p %u",
			prim_1mhz_ch_idx, oper_freq, oper_chwidth, prim_chwidth);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set channel parameters on ifname %s (%d)",
			ifname, ret);

	return ret;
}

int morse_set_ecsa_params(const char *ifname, u8 global_oper_class, u8 prim_chwidth,
				int oper_chwidth, int oper_freq, u8 prim_1mhz_ch_idx,
				u8 prim_global_op_class, u32 s1g_capab)
{
	int ret;

	ret = morse_cli(ifname, "ecsa_info -g %d -p %u -n %d -o %d -c %d -l %d -s %d",
				global_oper_class, prim_chwidth, prim_1mhz_ch_idx,
				oper_chwidth, oper_freq, prim_global_op_class, s1g_capab);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set ecsa parameters on ifname %s",
			ifname);

	return ret;
}

int morse_set_mbssid_info(const char *ifname, const char *tx_iface, u8 max_bss_index)
{
	int ret;

	ret = morse_cli(ifname, "mbssid -t %s -m %d",
				tx_iface,
				max_bss_index);

	if (ret != 0) {
		wpa_printf(MSG_WARNING,
				"morse: Failed to execute morse_cli to set MBSSID parameters on ifname %s",
				ifname);
	}
	return ret;
}

#if CONFIG_MORSE_STANDBY_MODE
void morse_standby_session_store(const char *ifname, const u8 *bssid,
					const char *standby_session_dir)
{
	int ret;

	if (!standby_session_dir)
		return;

	ret = morse_cli(ifname, "standby store -b " MACSTR " -d %s\n",
		MAC2STR(bssid), standby_session_dir);
	if (ret < 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to store standby session on ifname %s",
			ifname);
}
#endif /* CONFIG_MORSE_STANDBY_MODE */

int morse_set_keep_alive(const char *ifname, u16 bss_max_idle_period, bool as_11ah)
{
	int ret;

	ret = morse_cli(ifname, "keepalive %d%s", bss_max_idle_period, (as_11ah) ? " -a" : "");
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set bss max idle period on ifname %s",
			ifname);

	return ret;
}

int morse_twt_conf(const char *ifname, struct morse_twt *twt_config)
{
	int ret;

	ret = morse_cli(NULL, "twt conf -w %lld -d %d -c %d",
			twt_config->wake_interval_us, twt_config->wake_duration_us,
			twt_config->setup_command);

	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set twt config on ifname %s",
			ifname);
	else
		wpa_printf(MSG_INFO, "TWT config set successfully");

	return ret;
}

int morse_cac_conf(const char *ifname, bool enable)
{
	int ret;

	ret = morse_cli(ifname, "cac %s", enable ? "enable" : "disable");
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli cac enable command on ifname %s",
			ifname);

	return ret;
}

int morse_set_mesh_config(const char *ifname, u8 *mesh_id, u8 mesh_id_len, u8 beaconless_mode,
	u8 max_plinks)
{
	int ret;
	char mesh_id_hex[SSID_MAX_LEN * 2 + 1];

	wpa_snprintf_hex(mesh_id_hex, sizeof(mesh_id_hex), mesh_id, mesh_id_len);
	wpa_printf(MSG_DEBUG, "morse: Mesh ID:%s Mesh ID hex:%s",
			wpa_ssid_txt(mesh_id, mesh_id_len), mesh_id_hex);

	ret = morse_cli(ifname, "mesh_config -m %s -b %d -p %d",
			mesh_id_hex, beaconless_mode, max_plinks);

	if (ret != 0) {
		wpa_printf(MSG_WARNING,
				"morse: Failed to execute morse_cli to set Mesh Config %s",
				ifname);
	}
	return ret;
}

int morse_mbca_conf(const char *ifname, u8 mbca_config, u8 min_beacon_gap, u8 tbtt_adj_interval,
	u8 beacon_timing_report_interval, u16 mbss_start_scan_duration)
{
	int ret;

	ret = morse_cli(ifname, "mbca -m %u -s %u -r %u -g %u -i %u", mbca_config,
		mbss_start_scan_duration, beacon_timing_report_interval, min_beacon_gap,
		tbtt_adj_interval);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli mbca command on ifname %s", ifname);
	return ret;
}


int morse_set_mesh_dynamic_peering(const char *ifname, bool enabled, u8 rssi_margin,
	u32 blacklist_timeout)
{
	int ret;
	const char *operation = enabled ? "enable" : "disable";

	if (enabled)
		ret = morse_cli(ifname, "dynamic_peering %s -r %u -t %u", operation, rssi_margin,
			blacklist_timeout);
	else
		ret = morse_cli(ifname, "dynamic_peering %s", operation);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"%s: Failed to execute morse_cli dynamic_peering command on ifname %s",
				__func__,
				ifname);
	return ret;
}

int morse_raw_global_enable(const char *ifname, bool enable)
{
	int ret;
	const char *op = enable ? "enable" : "disable";
	const int global_raw_idx = 0;

	ret = morse_cli(ifname, "raw %s %d", op, global_raw_idx);

	if (ret != 0)
		wpa_printf(MSG_WARNING, "morse: Failed to execute morse_cli to %s RAW on ifname %s",
				op, ifname);

	return ret;
}

static void morse_raw_prio_to_aid_range(uint8_t prio, uint16_t *aid_start, uint16_t *aid_end)
{
	if (prio == 0) {
		*aid_start = MORSE_RAW_DEFAULT_START_AID;
		*aid_end = (__UINT16_MAX__ & MORSE_RAW_AID_DEVICE_MASK);
	} else if ((prio > 0) && (prio < (MORSE_MAX_NUM_RAWS_USER_PRIO - 1))) {
		*aid_start = (prio << MORSE_RAW_AID_PRIO_SHIFT);
		*aid_end = *aid_start + (__UINT16_MAX__ & MORSE_RAW_AID_DEVICE_MASK);
	} else if (prio == (MORSE_MAX_NUM_RAWS_USER_PRIO - 1)) {
		*aid_start = (prio << MORSE_RAW_AID_PRIO_SHIFT);
		*aid_end = MAX_AID;
	} else {
		WPA_ASSERT(false);
	}
}

static inline u16 morse_raw_prio_to_raw_idx(u16 prio)
{
	return prio + MORSE_RAW_ID_HOSTAPD_PRIO_OFFSET;
}

int morse_raw_priority_enable(const char *ifname, bool enable, u8 prio, u32 start_time_us,
	u32 duration_us, u8 num_slots, bool cross_slot, u16 max_bcn_spread, u16 nom_stas_per_bcn,
	u8 praw_period, u8 praw_start_offset)
{
	int ret;
	char buff[64] = {0};
	uint16_t aid_start = 0;
	uint16_t aid_end = 0;

	if (enable) {
		morse_raw_prio_to_aid_range(prio, &aid_start, &aid_end);
		if (nom_stas_per_bcn)
			snprintf(buff, sizeof(buff), "-b%u,%u", max_bcn_spread, nom_stas_per_bcn);
		else if (praw_period)
			snprintf(buff, sizeof(buff), "-p%u,-1,%u", praw_period, praw_start_offset);

		ret = morse_cli(ifname, "raw -s %u,%u%s -a %u,%u -t %u %s enable %u",
						duration_us,
						num_slots,
						cross_slot ? " -x" : "",
						aid_start,
						aid_end,
						start_time_us,
						buff,
						morse_raw_prio_to_raw_idx(prio));

	} else {
		ret = morse_cli(ifname, "raw disable %u", morse_raw_prio_to_raw_idx(prio));
	}
	if (ret != 0)
		wpa_printf(MSG_WARNING,
				"morse: Failed to execute morse_cli to set RAW priority %u on ifname %s (ret %d)",
				prio, ifname, ret);

	return ret;
}

int morse_set_bss_color(const char *ifname, u8 s1g_bss_color)
{
	int ret;

	if (!s1g_bss_color)
		return 0;

	ret = morse_cli(ifname, "bsscolor %d", s1g_bss_color);
	if (ret != 0)
		wpa_printf(MSG_WARNING,
			"morse: Failed to execute morse_cli to set bss color on ifname %s (%d)",
			ifname, ret);

	return ret;
}

int morse_set_active_scan_dwell_ms(const char *ifname, int dwell_ms)
{
	int ret;

	if (dwell_ms < 0)
		return -1;

	ret = morse_cli(ifname, "set active_scan_dwell_ms %d", dwell_ms);
	if (ret != 0)
		wpa_printf(MSG_WARNING, "%s: failed (ret:%d)", __func__, ret);

	return ret;
}

/**
 * Execute a morse_cli get command line.
 *
 * @param iface Optional interface name. If specified then "-i <iface>" will be included in
 *			the command line.
 * @param out_buf Output buffer
 * @param out_size size of output buffer in bytes
 * @param args_fmt printf format string for morse_cli arguments.
 * @param ... Variadic arguments for format string.
 *
 * @returns 0 on success, else an error code.
 */
static int morse_cli_get(const char *ifname, char *out_buf, size_t out_size,
				const char *args_fmt, ...)
{
	int ret;
	char cmd[MORSE_CLI_CMDLINE_MAXLEN];
	va_list vargs;
	FILE *p;
	char *buf = cmd;
	size_t buf_size = sizeof(cmd);

	if (!out_buf || out_size == 0) {
		wpa_printf(MSG_ERROR, "morse: morse_cli get invalid output buffer");
		return -1;
	}
	out_buf[0] = '\0';

	if (ifname && ifname[0])
		ret = snprintf(buf, buf_size, "%s -i %s ", MORSE_CLI_COMMAND, ifname);
	else
		ret = snprintf(buf, buf_size, "%s ", MORSE_CLI_COMMAND);

	if (ret < 0 || (size_t)ret >= buf_size) {
		wpa_printf(MSG_ERROR, "morse: morse_cli get cmd overflow, ret=%d", ret);
		return -1;
	}
	buf += (size_t)ret;
	buf_size -= (size_t)ret;

	va_start(vargs, args_fmt);
	ret = vsnprintf(buf, buf_size, args_fmt ? args_fmt : "", vargs);
	va_end(vargs);

	if (ret < 0 || (size_t)ret >= buf_size) {
		wpa_printf(MSG_ERROR, "morse: morse_cli get cmd args overflow");
		return -1;
	}

	p = popen(cmd, "r");
	if (!p) {
		wpa_printf(MSG_ERROR, "morse: popen failed for '%s'", cmd);
		return -1;
	}

	if (!fgets(out_buf, (int)out_size, p)) {
		wpa_printf(MSG_ERROR, "morse: fgets failed for '%s'", cmd);
		pclose(p);
		return -1;
	}
	pclose(p);

	/* remove trailing newline (and CR if ever present) */
	out_buf[strcspn(out_buf, "\r\n")] = '\0';
	return 0;
}

int morse_get_channelization(const char *ifname, u32 *channelization)
{
	char buf[32] = {0};
	char *end = NULL;
	unsigned long value;

	if (!channelization) {
		wpa_printf(MSG_ERROR, "morse: channelization buffer NULL");
		return -1;
	}

	if (morse_cli_get(ifname, buf, sizeof(buf), "get channelization") < 0)
		return -1;

	value = strtoul(buf, &end, 10);
	if (end == buf || *end != '\0') {
		wpa_printf(MSG_ERROR, "morse: invalid channelization '%s'", buf);
		return -1;
	}

	if (value > CHANNELIZATION_SCHEME_IEEE80211_REVMF) {
		wpa_printf(MSG_ERROR, "morse: invalid channelization value %lu", value);
		return -1;
	}

	*channelization = (u32)value;
	wpa_printf(MSG_DEBUG, "morse: channelization scheme %d used on ifname %s\n",
			*channelization, ifname);
	return 0;
}
