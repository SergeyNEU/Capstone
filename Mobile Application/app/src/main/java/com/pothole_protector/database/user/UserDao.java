package com.pothole_protector.database.user;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;


@Dao
public interface UserDao {

    @Insert
    void insertUser(User user);

    @Delete
    void deleteUser(User user);

    @Query("SELECT * FROM user where username = :username")
    User getUser(String username);

}
