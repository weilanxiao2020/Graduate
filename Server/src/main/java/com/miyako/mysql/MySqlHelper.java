package com.miyako.mysql;

import com.alibaba.druid.pool.DruidDataSource;
import com.alibaba.druid.pool.DruidDataSourceFactory;
import com.miyako.utils.LogUtil;

import javax.sql.DataSource;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.sql.*;
import java.util.*;

/**
 * ClassName MySqlHelper
 * Description //TODO
 * Author Miyako
 * Date 2020-03-28-0028 14:28
 */
public class MySqlHelper {

    private static final String TAG = MySqlHelper.class.getSimpleName();

    private static final class Obj {
        private static final MySqlHelper obj = new MySqlHelper();
    }

    private MySqlHelper() {}

    private String jdbc;
    private String url;
    private String username;
    private String password;

    private boolean isPool;

    private DataSource dataSource;
    //private Connection connection;
    //private PreparedStatement preparedStatement;
    //private ResultSet resultSet;

    public static MySqlHelper getInstance() {
        synchronized (MySqlHelper.class) {
            return Obj.obj;
        }
    }

    public MySqlHelper init(String jdbc, String url, String username, String password) {
        init(jdbc, url, username, password, false);
        return this;
    }

    public MySqlHelper init(String jdbc, String url, String username, String password, boolean isPool) {
        this.jdbc = jdbc;
        this.url = url;
        this.username = username;
        this.password = password;
        this.isPool = isPool;
        getDataSource();
        return this;
    }

    private void getDataSource() {
        try {
            LogUtil.i(TAG, "连接数据库...");
            // 注册 JDBC 驱动
            //Class.forName(jdbc);
            // 打开链接
            //connection = DriverManager.getConnection(url, username, password);
            Properties pro = new Properties();
            if (isPool) {
                LogUtil.i(TAG, "使用druid连接池...");
                pro.load(new InputStreamReader(Object.class.getResourceAsStream("/druid.properties"), StandardCharsets.UTF_8));
            }
            pro.put("driverClassName", jdbc);
            pro.put("url", url);
            pro.put("username", username);
            pro.put("password", password);
            dataSource = DruidDataSourceFactory.createDataSource(pro);//获取连接池
            //3.从数据库连接池获取数据库连接
            // 首次从连接池获取连接时，会初始化话连接池中的连接数
        } catch (Exception e) {
            e.printStackTrace();
            LogUtil.e(TAG, "获取数据库连接失败:"+e.getMessage());
        }
    }

    private Connection getConnection() {
        try {
           return dataSource.getConnection();//获取连接
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    //update/delete/insert
    //sql格式:UPDATE tablename SET columnn = ? WHERE column = ?
    public int executeUpdate(String sql, String[] parameters) {
        LogUtil.i(TAG, "数据库修改操作...");
        int result = -1;
        Connection connection = null;
        PreparedStatement preparedStatement = null;
        ResultSet resultSet = null;
        try {
            //1.创建一个ps
            connection = getConnection();
            if (connection == null) {
                LogUtil.w(TAG, "获取数据库连接异常");
                return -1;
            }
            LogUtil.i(TAG, "开启事务...");
            connection.setAutoCommit(false);
            preparedStatement = connection.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
            //给？赋值
            if(parameters != null)
                for (int i =0; i < parameters.length; i++) {
                    preparedStatement.setString(i + 1, parameters[i]);
                }
            // 执行
            result = preparedStatement.executeUpdate();
            if (result > 0) {
                ResultSet rs = preparedStatement.getGeneratedKeys();
                if (rs.next()) {
                    result = rs.getInt(1);
                }
            }
        } catch(SQLException e) {
            e.printStackTrace();// 开发阶段
            result = -1;
            LogUtil.i(TAG, "数据库操作失败...");
            //回滚事务
            try {
                LogUtil.i(TAG, "尝试回滚...");
                connection.rollback();
            } catch (SQLException e1) {
                e1.printStackTrace();
                LogUtil.i(TAG, "回滚失败...");
            }
            e.printStackTrace();
        } finally{
            //关闭资源
            try {
                LogUtil.d(TAG, "提交事务...");
                if (connection != null) {
                    connection.commit();
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
            close(resultSet, preparedStatement, connection);
        }
        return result;
    }

    //select
    public List<Map<String, String>> executeQuery(String sql, String[] parameters){
        LogUtil.i(TAG, "数据库查询操作...");
        List<Map<String, String>> list = null;
        Connection connection = null;
        PreparedStatement preparedStatement = null;
        ResultSet rs= null;
        try {
            connection = getConnection();
            if (connection == null) {
                LogUtil.w(TAG, "获取数据库连接异常");
                return list;
            }
            preparedStatement = connection.prepareStatement(sql);
            if(parameters != null) {
                for (int i =0; i < parameters.length; i++) {
                    preparedStatement.setString(i + 1, parameters[i]);
                }
            }
            rs = preparedStatement.executeQuery();
            list = new ArrayList<>(rs.getFetchSize());
            while (rs.next()) {
                ResultSetMetaData metaData = rs.getMetaData();
                int cnt = metaData.getColumnCount();
                Map<String, String> map = new HashMap<>(cnt);
                for(int i =1;i<cnt+1;i++) {
                    map.put(metaData.getColumnName(i), rs.getString(metaData.getColumnName(i)));
                }
                list.add(map);
            }
            LogUtil.i(TAG, "返回查询结果");
        } catch(SQLException e) {
            e.printStackTrace();
            LogUtil.e(TAG, "数据库查询失败:"+e.getMessage());
            rs = null;
        } finally {
            close(rs, preparedStatement, connection);
        }
        return list;
    }

    private void close(ResultSet rs, Statement ps, Connection conn){
        try {
            if (rs !=null) rs.close();
            rs = null;
            if (ps !=null) ps.close();
            ps = null;
            if (conn !=null) conn.close();
            conn = null;
        }catch (SQLException e) {
            LogUtil.e(TAG, "关闭连接异常:"+e.getMessage());
            e.printStackTrace();
        }
    }

}
