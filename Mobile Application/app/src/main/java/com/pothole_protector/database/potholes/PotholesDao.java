package com.pothole_protector.database.potholes;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

@Dao
public interface PotholesDao {

@Insert
void insertPothole(Potholes pothole);


@Delete
void removePothole(Potholes pothole);

@Query("DELETE FROM potholes where uid = :potholeID")
void removePotholeByID(int potholeID);

@Query("SELECT * FROM potholes where uid = :potholeID")
Potholes getPotholeByID(int potholeID);


}
