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
        ((TextView) holder.getView(R.id.tv_order_gps))
                .setText(mContext.
                        getString(R.string.tv_mission_gps,
                                data.getLatitude(), data.getN_S(),
                                data.getLongitude(), data.getE_W()));
        ((TextView) holder.getView(R.id.tv_order_date))
                .setText(mContext.getString(R.string.tv_mission_gps_date, timeStamp2Date(data.getUtcTime()).split("-")[0]));
        ((TextView) holder.getView(R.id.tv_order_time))
                .setText(mContext.getString(R.string.tv_mission_gps_time, timeStamp2Date(data.getUtcTime()).split("-")[1]));
    }

    private static final SimpleDateFormat sdf = new SimpleDateFormat("MM/dd-HH:mm");;
    private static String timeStamp2Date(long seconds) {
        synchronized (sdf) {
            return sdf.format(new Date(seconds));
        }
    }
}
