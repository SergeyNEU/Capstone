package com.pothole_protector.database;

import android.content.Context;

import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;

import com.pothole_protector.database.devices.Devices;
import com.pothole_protector.database.devices.DevicesDao;
import com.pothole_protector.database.potholes.Potholes;
import com.pothole_protector.database.potholes.PotholesDao;
import com.pothole_protector.database.user.User;
import com.pothole_protector.database.user.UserDao;


@Database(entities = {
        Devices.class,
        Potholes.class,
        User.class },
        version = 1, exportSchema = false)
public abstract class AppDatabase extends RoomDatabase {
    public abstract UserDao userDao();
    public abstract DevicesDao devicesDao();
    public abstract PotholesDao potholesDao();

    private static volatile AppDatabase databaseInstance;

    public static  AppDatabase getDatabase(final Context context){
        databaseInstance = Room.databaseBuilder(context.getApplicationContext(), AppDatabase.class, "pothole_protector.db").build();
        return databaseInstance;
    }


}
