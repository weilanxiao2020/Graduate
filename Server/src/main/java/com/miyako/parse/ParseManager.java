package com.miyako.parse;

import com.miyako.utils.LogUtil;

/**
 * ClassName ParseManager
 * Description //TODO
 * Author Miyako
 * Date 2020-05-21-0021 14:34
 */
public class ParseManager {

    private static final String TAG = ParseManager.class.getSimpleName();

    public static final int PARSE_SQL = 0x01;
    public static final int PARSE_SOCKET = 0x02;
    public static final int PARSE_MQTT = 0x03;

    //public static void parse(int flag, Object obj) {
    //    LogUtil.d(TAG, "解析数据");
    //    switch(flag) {
    //        case PARSE_SQL:
    //            ParseDao.parse();
    //            break;
    //        case PARSE_SOCKET:
    //            break;
    //        case PARSE_MQTT:
    //            break;
    //        default:
    //            LogUtil.e(TAG, "解析调用错误");
    //            break;
    //    }
    //}
}
