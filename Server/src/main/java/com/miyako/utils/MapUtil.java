package com.miyako.utils;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import sun.rmi.runtime.Log;

import java.net.URL;
import java.util.HashMap;
import java.util.Map;

/**
 * ClassName MapUtil
 * Description //TODO
 * Author Miyako
 * Date 2020-04-21-0021 13:43
 */
public class MapUtil{

    private static final String host = "https://regeo.market.alicloudapi.com";
    private static final String path = "/v3/geocode/regeo";
    private static final String method = "GET";
    private static final String appcode = "2e2fe380ce6f45babcf5745c3409e35e";
    private static final String TAG = MapUtil.class.getSimpleName();

    public static synchronized String gpsParse(double latitude, double longitude) {

        Map<String, String> headers = new HashMap<String, String>();
        //最后在header中的格式(中间是英文空格)为Authorization:APPCODE 83359fd73fe94948385f570e3c139105
        headers.put("Authorization", "APPCODE " + appcode);
        //String location = String.format("%s,%s",107.0522403700, 29.8865693700);
        String location = String.format("%s,%s", longitude,latitude);
        Map<String, String> querys = new HashMap<String, String>();
        querys.put("batch", "false");
        querys.put("extensions", "base");
        querys.put("location", location);
        querys.put("output", "json");
        querys.put("radius", "1000");

        String urlString = host+path+"?"+param(querys);
        StringBuilder res = new StringBuilder();
        String region = "none";
        try {
            URL url = new URL(urlString);
            java.net.HttpURLConnection conn = (java.net.HttpURLConnection)url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod(method);
            // 设置通用的请求属性
            for (Map.Entry<String, String> entry : headers.entrySet()) {
                System.out.println(entry.getKey() + ":" + entry.getValue());
                conn.setRequestProperty(entry.getKey(), entry.getValue());
            }
            java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(conn.getInputStream(),"UTF-8"));
            String line;
            while ((line = in.readLine()) != null) {
                res.append(line).append("\n");
            }
            in.close();
            region = parseJson(res.toString());
        } catch (Exception e) {
            LogUtil.d(TAG, "error "+ e.getMessage());
        }
        return region;
    }

    private static String param(Map<String, String> querys) {
        StringBuilder stringBuilder = new StringBuilder();
        // 设置通用的请求属性
        if (querys!=null) {
            for (Map.Entry<String, String> entry : querys.entrySet()) {
                stringBuilder.append(String.format("%s=%s&", entry.getKey(), entry.getValue()));
            }
        }
        return stringBuilder.toString();
    }

    private static String parseJson(String json) {
        // json解析器，解析json数据
        JsonParser parser = new JsonParser();
        JsonElement element = parser.parse(json);
        // json属于对象类型时
        String region = "none";
        if (element.isJsonObject()) {
            JsonObject object = element.getAsJsonObject();  // 转化为对象
            // 1. value为string时，取出string
            int code = object.get("infocode").getAsInt();
            System.out.println("infocode:"+code);
            if(code!=10000) {
                LogUtil.d(TAG, "请求错误");
                return region;
            }
            JsonObject regeocode = object.getAsJsonObject("regeocode");  // 转化为对象
            JsonObject addressComponent = regeocode.getAsJsonObject("addressComponent");
            String province = addressComponent.get("province").getAsString();
            if (!addressComponent.get("city").isJsonArray()) {
                String city = addressComponent.get("city").getAsString();
                //System.out.println(province+"-"+city);
                region = String.format("%s-%s",province, city);
            }else {
                String district= addressComponent.get("district").getAsString();
                //System.out.println(province+"-"+district);
                region = String.format("%s-%s",province, district);
            }
        }
        return region;
    }
}
