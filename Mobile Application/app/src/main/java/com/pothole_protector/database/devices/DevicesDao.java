package com.pothole_protector.database.devices;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;


@Dao
public interface DevicesDao {

    @Insert
    void insertDevice(Devices device);

    @Query("DELETE FROM devices where deviceID = :device")
    void removeDeviceByID(int device);

    @Query("SELECT * FROM devices where deviceID = :device")
    Devices getDeviceByDeviceID(int device);

    @Query("SELECT * FROM devices where userID = :user")
    Devices getDeviceByUSerID(int user);

}
