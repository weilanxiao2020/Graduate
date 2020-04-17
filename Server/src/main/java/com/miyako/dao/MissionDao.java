package com.miyako.dao;

import com.miyako.model.Mission;
import com.miyako.mysql.MySqlHelper;
import com.miyako.utils.LogUtil;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * ClassName MissionDao
 * Description //TODO
 * Author Miyako
 * Date 2020-03-28-0028 14:48
 */
public class MissionDao {

    private static final String TAG = MissionDao.class.getSimpleName();

    private static final String SQL_SELECT_ALL =
            "SELECT tb_mission.`id`, tb_mission.`timestamp`, tb_mission.`license`, tb_mission.`code`, tb_mission.`address`, tb_mission.`status`"
                    + "FROM tb_mission;";

    public static List<Mission> findAll() {
        LogUtil.d(TAG, "findAll");
        ResultSet resultSet = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ALL, null);
        List<Mission> missions = create(resultSet);
        return missions;
    }

    private static final String SQL_SELECT_ID =
            "SELECT tb_mission.`id`, tb_mission.`timestamp`, tb_mission.`license`, tb_mission.`code`, tb_mission.`address`, tb_mission.`status`"
                    +"FROM tb_mission WHERE tb_mission.`id` = ?;";
    public static Mission findById(int id) {
        LogUtil.d(TAG, "findById: "+id);
        ResultSet resultSet = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ID, new String[]{String.valueOf(id)});
        List<Mission> missions = create(resultSet);
        return missions.size()==0?null:missions.get(0);
    }

    private static final String SQL_SELECT_CARD =
            "SELECT tb_mission.`id`, tb_mission.`timestamp`, tb_mission.`license`, tb_mission.`code`, tb_mission.`address`, tb_mission.`status`"
                    +"FROM tb_mission WHERE tb_mission.`license` = ? AND tb_mission.`code` = ?;";
    public static Mission findByCard(String card) {
        LogUtil.d(TAG, "findByCard: "+card);
        String[] split = card.split("-");
        ResultSet resultSet = MySqlHelper.getInstance().executeQuery(SQL_SELECT_CARD, (split.length<2?(new String[]{"",""}) : split));
        List<Mission> missions = create(resultSet);
        return missions.size()==0?null:missions.get(0);
    }

    private static final String SQL_INSERT =
            "INSERT INTO tb_mission(`timestamp`, `license`, `code`, `address`,`status`) " +
                    "VALUES (?,?,?,?,?);";
    public static int insert(Mission mission) {
        LogUtil.d(TAG, "insert: "+mission);
        Mission temp = findByCard(mission.getLicense() + "-" + mission.getCode());
        if (temp != null) {
            LogUtil.d(TAG, "任务唯一标识已存在");
            return -1;
        }
        String[] array = new String[5];
        array[0] = String.valueOf(mission.getTimestamp());
        array[1] = mission.getLicense();
        array[2] = mission.getCode();
        array[3] = mission.getAddress();
        array[4] = mission.getStatus();
        return MySqlHelper.getInstance().executeUpdate(SQL_INSERT, array);
    }

    private static final String SQL_UPDATE =
            "UPDATE tb_mission SET `timestamp` = ?, `license` = ?, `code` = ?, `address` = ?, `status` =  ?" +
                    "WHERE `id` = ?;";
    public static int update(Mission mission) {
        LogUtil.d(TAG, "update: "+mission);
        if (mission.getId()==0) {
            LogUtil.d(TAG, "数据错误");
            return -1;
        }

        String[] array = new String[6];
        array[0] = String.valueOf(mission.getTimestamp());
        array[1] = mission.getLicense();
        array[2] = mission.getCode();
        array[3] = mission.getAddress();
        array[4] = mission.getStatus();
        array[5] = String.valueOf(mission.getId());
        return MySqlHelper.getInstance().executeUpdate(SQL_UPDATE, array);
    }

    private static List<Mission> create(ResultSet resultSet) {
        List<Mission> datas = null;
        try{
            datas = new ArrayList<>(resultSet.getFetchSize());
            while (resultSet.next()) {
                Mission mission = new Mission();
                mission.setId(resultSet.getInt("id"));
                mission.setTimestamp(resultSet.getLong("timestamp"));
                mission.setLicense(resultSet.getString("license"));
                mission.setCode(resultSet.getString("code"));
                mission.setAddress(resultSet.getString("address"));
                mission.setStatus(resultSet.getString("status"));
                datas.add(mission);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return datas;
    }
}
