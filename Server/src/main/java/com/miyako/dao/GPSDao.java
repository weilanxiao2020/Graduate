package com.miyako.dao;

import com.miyako.model.GPS;
import com.miyako.mysql.MySqlHelper;
import com.miyako.utils.LogUtil;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * ClassName MissionDao
 * Description //TODO
 * Author Miyako
 * Date 2020-03-28-0028 14:48
 */
public class GPSDao {

    private static final String TAG = GPSDao.class.getSimpleName();

    private static final String SQL_SELECT_ALL =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`, " +
            "tb_gps.`region`, tb_gps.`missionId`"
                    + "FROM tb_gps;";

    public static List<GPS> findAll() {
        LogUtil.d(TAG, "findAll");
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ALL, null);
        List<GPS> GPSs = create(mapList);
        return GPSs;
    }

    private static final String SQL_SELECT_ID =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`, " +
            "tb_gps.`region`, tb_gps.`missionId`"
                    +"FROM tb_gps WHERE tb_gps.`id` = ?;";
    public static GPS findById(int id) {
        LogUtil.d(TAG, "findById: "+id);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ID, new String[]{String.valueOf(id)});
        List<GPS> GPSs = create(mapList);
        return GPSs.size()==0?null:GPSs.get(0);
    }

    private static final String SQL_SELECT_TRACK =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`, tb_gps.`region`, tb_gps.`trackId`, tb_gps.`missionId`"
                    +"FROM tb_gps WHERE tb_gps.`trackId` = ?;";
    public static GPS findByTrackId(int track) {
        LogUtil.d(TAG, "findByTrackId: "+track);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_TRACK, new String[]{String.valueOf(track)});
        List<GPS> gpsList = create(mapList);
        return gpsList.size()==0?null:gpsList.get(0);
    }

    private static final String SQL_SELECT_MISSION =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`, " +
            "tb_gps.`region`, tb_gps.`missionId`"
                    +"FROM tb_gps WHERE tb_gps.`missionId` = ?;";
    public static List<GPS> findByMissionId(String mission) {
        LogUtil.d(TAG, "findByMissionId: "+mission);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_MISSION, new String[]{mission});
        List<GPS> gpsList = create(mapList);
        return gpsList.size()==0?null:gpsList;
    }

    private static final String SQL_SELECT_LAST =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`, " +
            "tb_gps.`region`, tb_gps.`missionId`"
            + "FROM tb_gps WHERE tb_gps.`missionId` = ? ORDER BY tb_gps.`timestamp` DESC limit 1;";
    public static GPS findByMissionIdLast(String mission) {
        LogUtil.d(TAG, "findByMissionIdLast:"+mission);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_LAST, new String[]{mission});
        List<GPS> gpsList = create(mapList);
        return gpsList.size()==0?null:gpsList.get(0);
    }

    private static final String SQL_SELECT_LAST_BY_REGION =
            "SELECT tb_gps.`id`, tb_gps.`timestamp`, tb_gps.`latitude`, tb_gps.`longitude`,tb_gps.`region`, tb_gps.`missionId` " +
            "FROM tb_gps RIGHT JOIN (SELECT tb_gps.region, MAX(tb_gps.`timestamp`) as t FROM tb_gps GROUP BY tb_gps.region) tmp " +
            "ON tmp.region = tb_gps.`region` AND tmp.t = tb_gps.`timestamp` " +
            "WHERE tb_gps.`missionId` = ? ORDER BY tb_gps.`timestamp` DESC;";
    public static List<GPS> findByMissionIdRegion(String mission) {
        LogUtil.d(TAG, "findByMissionIdRegion:"+mission);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_LAST_BY_REGION, new String[]{mission});
        List<GPS> gpsList = create(mapList);
        return gpsList.size()==0?null:gpsList;
    }

    private static final String SQL_INSERT =
            "INSERT INTO tb_gps(`timestamp`, `latitude`, `longitude`, `region`,`missionId`) " +
                    "VALUES (?,?,?,?,?);";
    public static int insert(GPS gps) {
        LogUtil.d(TAG, "insert: "+gps);
//        GPS temp = findByTrackId(gps.getTrackId());
//        if (temp != null) {
//            LogUtil.d(TAG, "订单唯一标识已存在");
//            return -1;
//        }
        String[] array = new String[5];
        array[0] = String.valueOf(gps.getTimestamp());
        array[1] = gps.getLatitude();
        array[2] = gps.getLongitude();
        array[3] = gps.getRegion();
        array[4] = gps.getMissionId();
        return MySqlHelper.getInstance().executeUpdate(SQL_INSERT, array);
    }

    private static final String SQL_UPDATE =
            "UPDATE tb_gps SET `timestamp` = ?, `latitude` = ?, `longitude` = ?, `region` = ?, `missionId` = ?" +
                    "WHERE `id` = ?;";
    public static int update(GPS gps) {
        LogUtil.d(TAG, "update: "+gps);
        if (gps.getId()==0) {
            LogUtil.w(TAG, "数据错误");
            return -1;
        }

        String[] array = new String[6];
        array[0] = String.valueOf(gps.getTimestamp());
        array[1] = gps.getLatitude();
        array[2] = gps.getLongitude();
        array[3] = gps.getRegion();
        array[4] = String.valueOf(gps.getMissionId());
        array[5] = String.valueOf(gps.getId());
        return MySqlHelper.getInstance().executeUpdate(SQL_UPDATE, array);
    }

    private static List<GPS> create(List<Map<String, String>> mapList) {
        List<GPS> datas = new ArrayList<>(mapList.size());
        for(Map<String, String> map : mapList) {
            GPS gps = new GPS();
            gps.setId(Integer.valueOf(map.get("id")));
            gps.setTimestamp(Long.valueOf(map.get("timestamp")));
            gps.setLatitude(map.get("latitude"));
            gps.setLongitude(map.get("longitude"));
            gps.setRegion(map.get("region"));
            gps.setMissionId(map.get("missionId"));
            datas.add(gps);
        }
        return datas;
    }
}
