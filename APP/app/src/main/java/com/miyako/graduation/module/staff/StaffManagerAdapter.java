package com.miyako.graduation.module.staff;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.miyako.graduation.R;
import com.miyako.graduation.base.CommonAdapter;
import com.miyako.graduation.base.CommonViewHolder;
import com.miyako.graduation.entity.StaffOrder;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * @Description
 * @Author Miyako
 * @Date 2020-02-29-0029
 */
public class StaffManagerAdapter extends CommonAdapter<StaffOrder> {


    public StaffManagerAdapter(Context context, List<StaffOrder> datas){
        super(context, datas);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.item_list_staff_order;
    }

    @Override
    public void setData(CommonViewHolder holder, StaffOrder data) {
        ((TextView) holder.getView(R.id.tv_order_id)).setText(data.getId());
        ((TextView) holder.getView(R.id.tv_order_address)).setText(data.getAddress());
        ((TextView) holder.getView(R.id.tv_order_receiver)).setText(data.getReceiver());
        ((TextView) holder.getView(R.id.tv_order_date)).setText(timeStamp2Date(data.getDate()).split("-")[0]);
        ((TextView) holder.getView(R.id.tv_order_time)).setText(timeStamp2Date(data.getDate()).split("-")[1]);
    }

    public static String timeStamp2Date(long seconds) {
        SimpleDateFormat sdf = new SimpleDateFormat("MM/dd-HH:mm");
        return sdf.format(new Date(seconds));
    }
}
