package com.miyako.graduate.module.staff;

import android.content.Context;
import android.widget.TextView;

import com.miyako.graduate.R;
import com.miyako.graduate.base.CommonAdapter;
import com.miyako.graduate.base.CommonViewHolder;
import com.miyako.graduate.entity.StaffOrder;

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
        ((TextView) holder.getView(R.id.tv_mission_license))
                .setText(mContext.getString(R.string.tv_order_trackId, data.getId()));
        ((TextView) holder.getView(R.id.tv_order_address))
                .setText(mContext.getString(R.string.tv_order_address, data.getAddress()));
        ((TextView) holder.getView(R.id.tv_mission_code))
                .setText(mContext.getString(R.string.tv_order_receiver, data.getReceiver()));
        ((TextView) holder.getView(R.id.tv_order_date))
                .setText(mContext.getString(R.string.tv_order_date, timeStamp2Date(data.getDate()).split("-")[0]));
        ((TextView) holder.getView(R.id.tv_order_time))
                .setText(mContext.getString(R.string.tv_order_time, timeStamp2Date(data.getDate()).split("-")[1]));
    }

    private static final SimpleDateFormat sdf = new SimpleDateFormat("MM/dd-HH:mm");

    private static String timeStamp2Date(long seconds) {
        synchronized (sdf) {
            return sdf.format(new Date(seconds));
        }
    }
}
