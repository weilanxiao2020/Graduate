package com.miyako.dao;

import com.miyako.mysql.MySqlHelper;
import com.miyako.model.Order;
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
public class OrderDao {

    private static final String TAG = OrderDao.class.getSimpleName();

    private static final String SQL_SELECT_ALL =
            "SELECT tb_order.`id`, tb_order.`timestamp`, tb_order.`trackId`, " +
            "tb_order.`sender`, tb_order.`receiver`, tb_order.`address`, tb_order.`status`, tb_order.`missionId`"
                    + "FROM tb_order;";

    public static List<Order> findAll() {
        LogUtil.d(TAG, "findAll");
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ALL, null);
        return create(mapList);
    }

    private static final String SQL_SELECT_ID =
            "SELECT tb_order.`id`, tb_order.`timestamp`, tb_order.`trackId`, " +
            "tb_order.`sender`, tb_order.`receiver`, tb_order.`address`, tb_order.`status`, tb_order.`missionId`"
                    +"FROM tb_order WHERE tb_order.`id` = ?;";
    public static Order findById(int id) {
        LogUtil.d(TAG, "findById: "+id);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_ID, new String[]{String.valueOf(id)});
        List<Order> orders = create(mapList);
        return orders.size() ==0 ? null : orders.get(0);
    }

    private static final String SQL_SELECT_TRACK =
            "SELECT tb_order.`id`, tb_order.`timestamp`, tb_order.`trackId`, " +
            "tb_order.`sender`, tb_order.`receiver`, tb_order.`address`, tb_order.`status`, tb_order.`missionId`"
                    +"FROM tb_order WHERE tb_order.`trackId` = ?;";
    public static Order findByTrackId(String track) {
        LogUtil.d(TAG, "findByTrackId: "+track);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_TRACK, new String[]{String.valueOf(track)});
        List<Order> orders = create(mapList);
        return orders.size()==0?null:orders.get(0);
    }

    private static final String SQL_SELECT_MISSION =
            "SELECT tb_order.`id`, tb_order.`timestamp`, tb_order.`trackId`, " +
            "tb_order.`sender`, tb_order.`receiver`, tb_order.`address`, tb_order.`status`, tb_order.`missionId`"
            +"FROM tb_order WHERE tb_order.`missionId` = ?;";
    public static List<Order> findByMissionId(String mission) {
        LogUtil.d(TAG, "findByMissionId: "+mission);
        List<Map<String, String>> mapList = MySqlHelper.getInstance().executeQuery(SQL_SELECT_MISSION, new String[]{String.valueOf(mission)});
        List<Order> orders = create(mapList);
        return orders;
    }

    private static final String SQL_INSERT =
            "INSERT INTO tb_order(`timestamp`, `trackId`, `sender`, `receiver`,`address`,`status`, `missionId`) " +
                    "VALUES (?,?,?,?,?,?,?);";
    public static int insert(Order order) {
        LogUtil.d(TAG, "insert: "+order);
        Order temp = findByTrackId(order.getTrackId());
        if (temp != null) {
            LogUtil.w(TAG, "订单唯一标识已存在");
            return -1;
        }
        String[] array = new String[7];
        array[0] = String.valueOf(order.getTimestamp());
        array[1] = order.getTrackId();
        array[2] = order.getSender();
        array[3] = order.getReceiver();
        array[4] = order.getAddress();
        array[5] = order.getStatus();
        array[6] = order.getMissionId();
        return MySqlHelper.getInstance().executeUpdate(SQL_INSERT, array);
    }

    private static final String SQL_UPDATE =
            "UPDATE tb_order SET `timestamp` = ?, `sender` = ?, `receiver` = ?, `address` =  ?, `status` = ?" +
                    "WHERE `id` = ?;";
    public static int update(Order order) {
        LogUtil.d(TAG, "update: "+order);
        if (order.getId()==0) {
            LogUtil.w(TAG, "数据错误");
            return -1;
        }

        String[] array = new String[6];
        array[0] = String.valueOf(order.getTimestamp());
        array[1] = order.getSender();
        array[2] = order.getReceiver();
        array[3] = order.getAddress();
        array[4] = order.getStatus();
        array[5] = String.valueOf(order.getId());
        return MySqlHelper.getInstance().executeUpdate(SQL_UPDATE, array);
    }

    private static List<Order> create(List<Map<String, String>> mapList) {
        List<Order> datas = new ArrayList<>(mapList.size());
        for (Map<String, String> map : mapList) {
            Order order = new Order();
            order.setId(Integer.valueOf(map.get("id")));
            order.setTimestamp(Long.valueOf(map.get("timestamp")));
            order.setTrackId(map.get("trackId"));
            order.setSender(map.get("sender"));
            order.setReceiver(map.get("receiver"));
            order.setAddress(map.get("address"));
            order.setStatus(map.get("status"));
            order.setMissionId(map.get("missionId"));
            datas.add(order);
        }
        return datas;
    }
}
