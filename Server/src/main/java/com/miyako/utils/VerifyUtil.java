package com.miyako.utils;

import com.google.gson.Gson;
import sun.rmi.runtime.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.regex.PatternSyntaxException;

/**
 * ClassName VerifyUtil
 * Description //TODO
 * Author Miyako
 * Date 2020-04-02-0002 15:12
 */
public class VerifyUtil {

    private static final String TAG = VerifyUtil.class.getSimpleName();

    private static Set<String> REGION;
    private static ProvinceCity provinceCity;

    private static final class ProvinceCity {
        public Province[] provinces;
    }

    private static final class Province {
        public long code;
        public String province;
        public City[] cities;
    }

    private static final class City {
        public long code;
        public String city;
        public District[] districts;
    }

    private static final class District {
        public long code;
        public String district;
    }

    public static void init() throws IOException{
        initRegion();
    }

    private static void initRegion() throws IOException{
        REGION = new HashSet<>();
        Gson gson = new Gson();
        BufferedReader reader = new BufferedReader(new InputStreamReader(
                Object.class.getResourceAsStream("/" + "ProvincesCities.json"), StandardCharsets.UTF_8));
        int ch = 0;
        StringBuilder sb = new StringBuilder();
        while ((ch = reader.read()) != -1) {
            sb.append((char) ch);
        }
        String json = sb.toString();
        reader.close();
        provinceCity = gson.fromJson(json, ProvinceCity.class);
        //for (int i =0;i<provinceCity.provinces.length;i++) {
        //    LogUtil.d(TAG, provinceCity.provinces[i].province);
        //}
    }

    public static String verfiyRegion(int adCode) {
        String code = adCode+"";
        for(int i =0;i<provinceCity.provinces.length;i++) {
            Province province = provinceCity.provinces[i];
            String pre = (province.code+"").substring(0,2);
            if (code.startsWith(pre)) {
                //LogUtil.d(TAG, "pre:"+pre);
                for(int j =0;j<province.cities.length;j++) {
                    City city = province.cities[j];
                    String pre_mid = (city.code+"").substring(0,4);
                    if(code.startsWith(pre_mid)) {
                        //LogUtil.d(TAG, "pre mid:"+pre_mid);
                        for (int k = 0; k < city.districts.length; k++) {
                            District district = city.districts[k];
                            //LogUtil.d(TAG, "code:"+district.code);
                            if ((district.code+"").startsWith(code)) {
                                String address = String.format("%s-%s-%s", province.province,city.city,district.district);
                                LogUtil.d(TAG, "匹配:"+address);
                                return address;
                            }
                        }
                    }
                }
            }
        }
        return "";
    }

    public static boolean verfiyRegion(String address) {
        String[] target;
        try{
            target = address.split("-");
            if (target.length != 2) {
                throw new PatternSyntaxException(null,null,0);
            }
        }catch (PatternSyntaxException ex) {
            LogUtil.e(TAG, "address is error");
            return false;
        }
        LogUtil.d(TAG, "verfiy region:"+target[0]+","+target[1]);
        if (verfiyBTSC(target)) {
            return true;
        }
        for(int i =0;i<provinceCity.provinces.length;i++) {
            Province province = provinceCity.provinces[i];
            if (province.province.contains(target[0])) {
                for(int j =0;j<province.cities.length;j++) {
                    City city = province.cities[i];
                    if (city.city.contains(target[1])) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private static boolean verfiyBTSC(String[] address) {
        LogUtil.d(TAG, "verfiy BTSC:");
        for(int i =0;i<provinceCity.provinces.length;i++) {
            Province province = provinceCity.provinces[i];
            if (province.province.contains(address[0])) {
                City city =province.cities[0];
                for(int k=0;k<city.districts.length;k++) {
                    if (city.districts[k].district.contains(address[1])) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
}
