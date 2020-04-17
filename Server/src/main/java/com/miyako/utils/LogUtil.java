package com.miyako.utils;

import org.apache.log4j.Logger;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * ClassName LogUtil
 * Description //TODO
 * Author Miyako
 * Date 2020-02-22-0022 16:44
 */
public class LogUtil {

    //private static Logger logger;
    private static Logger logger = Logger.getLogger(LogUtil.class);

    public static boolean DEBUG = false;

    private static String Debug = "Debug";
    private static String Info = "Info";
    private static String Wraning = "Warning";
    private static String Error = "Error";

    private static void print(String level, String tag, String msg) {
        String name = Thread.currentThread().getName();
        System.out.println(String.format("%s [%s] %s=>/ %s: %s", getTime(), name.length()>32?name.substring(0,32)+"...":name, level, tag, msg));
    }

    private static String format(String tag, String msg) {
        return String.format("[%s]:=> %s", tag, msg);
    }

    public static void d(String tag, String msg) {
        if (logger==null) {
            print(Debug, tag, msg);
        } else {
            logger.debug(format(tag, msg));
        }
    }

    public static void i(String tag, String msg) {
        if (logger==null) {
            print(Info, tag, msg);
        } else {
            logger.info(format(tag, msg));
        }
    }

    public static void w(String tag, String msg) {
        if (logger==null) {
            print(Wraning, tag, msg);
        } else {
            logger.warn(format(tag, msg));
        }

    }

    public static void e(String tag, String msg) {
        if (logger==null) {
            print(Error, tag, msg);
        } else {
            logger.error(format(tag, msg));
        }
    }


    //返回详细的时间字符串，精确到毫秒
    private static String getTime(){
        return new SimpleDateFormat("HH:mm:ss.SSS").format(new Date());
    }

}
