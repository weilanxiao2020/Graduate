package com.miyako.graduation.socket.msg;

/**
 * ClassName QueryMsg
 * Description //TODO
 * Author Miyako
 * Date 2020-04-04-0004 14:16
 */
public class QueryMsg extends BaseMsg {

    private String queryId;

    public String getQueryId(){
        return queryId;
    }

    public void setQueryId(String queryId){
        this.queryId = queryId;
    }

    @Override
    public String toString(){
        return "QueryMsg{" +
               "queryId='" + queryId + '\'' +
               '}';
    }
}
