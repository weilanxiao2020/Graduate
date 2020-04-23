package com.miyako.graduate.module.user;

import android.content.Context;
import android.widget.TextView;

import com.miyako.graduate.R;
import com.miyako.graduate.base.CommonAdapter;
import com.miyako.graduate.base.CommonViewHolder;
import com.miyako.graduate.entity.GpsOrder;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * @Description
 * @Author Miyako
 * @Date 2020-02-29-0029
 */
public class UserOrderAdapter extends CommonAdapter<GpsOrder> {


    public UserOrderAdapter(Context context, List<GpsOrder> datas){
        super(context, datas);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.item_list_user_order;
    }

    @Override
    public void setData(CommonViewHolder holder, GpsOrder data) {
        ((TextView) holder.getView(R.id.tv_order_id)).setText(data.getId()+"");
        ((TextView) holder.getView(R.id.tv_order_gps)).setText(data.getLatitude()+"["+data.getN_S()+"], "+data.getLongitude()+"["+data.getE_W()+"]");
        ((TextView) holder.getView(R.id.tv_order_date)).setText(timeStamp2Date(data.getUtcTime()).split("-")[0]);
        ((TextView) holder.getView(R.id.tv_order_time)).setText(timeStamp2Date(data.getUtcTime()).split("-")[1]);
    }

    public static String timeStamp2Date(long seconds) {
        SimpleDateFormat sdf = new SimpleDateFormat("MM/dd-HH:mm");
        return sdf.format(new Date(seconds));
    }
}
