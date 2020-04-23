package com.miyako.graduate.base;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.List;

/**
 * @Description
 * @Author Miyako
 * @Date 2020-03-12-0012
 */
public abstract class CommonAdapter<T> extends android.widget.BaseAdapter {

    protected Context mContext;
    protected LayoutInflater mInflater;
    protected final List<T> mDatas;
    protected int mLayoutId;

//    public static CommonAdapter create(Context context, List<BaseBean> datas, int layoutId) {
//        return new CommonAdapter(context, datas, layoutId);
//    }

    protected CommonAdapter(Context context, List<T> datas) {
        mContext = context;
        mInflater = LayoutInflater.from(mContext);
        mDatas = datas;
        mLayoutId = getLayoutId();
    }

    protected abstract int getLayoutId();

    public void update(List<T> datas) {
        mDatas.clear();
        mDatas.addAll(datas);
        notifyDataSetChanged();
    }

    @Override
    public int getCount() {
        return mDatas != null ? mDatas.size() : 0;
    }

    @Override
    public T getItem(int position) {
        return mDatas!=null?mDatas.get(position):null;
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        CommonViewHolder holder = CommonViewHolder.get(mContext, convertView, parent, mLayoutId, position);
        setData(holder, mDatas.get(position));
        return holder.getConvertView();
    }

    protected abstract void setData(CommonViewHolder holder, T t);

}
