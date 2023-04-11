package com.pothole_protector;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.room.EmptyResultSetException;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.pothole_protector.database.AppDatabase;
import com.pothole_protector.database.user.User;

import io.reactivex.schedulers.Schedulers;

public class LoginActivity extends AppCompatActivity {
    EditText username;
    EditText password;
    TextView signUp;
    Button signIn;

    AppDatabase db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        db = AppDatabase.getDatabase(this.getApplicationContext());

        //link fields to ui
        username = findViewById(R.id.username_field);
        password = findViewById(R.id.password_field);
        signUp = findViewById(R.id.sign_up);
        signIn = findViewById(R.id.sign_in);

        //set up click listeners
        signUp.setOnClickListener(l -> {addUser();});
        signIn.setOnClickListener(l -> {logInUser();});

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if(!Environment.isExternalStorageManager()) {
                Intent intent = new Intent();
                intent.setAction(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                Uri uri = Uri.fromParts("package", this.getPackageName(), null);
                intent.setData(uri);
                startActivity(intent);

            }
        }


    }

    @SuppressLint("CheckResult")
    private void logInUser() {
        String usernameText = username.getText().toString();
        String passwordText = password.getText().toString();
        User use = checkValidUser(usernameText);

        if(use == null || !passwordText.equals(use.password)){
            LoginActivity.showAlert("Invalid Credentials", "Either the password or username given is incorrect", "Okay", null, this);
            return;
        }
        launchBluetoothActivity();

    }

    public static void showAlert(String title, String message, String posText, String negText, Context context) {
        new AlertDialog.Builder(context)
                .setTitle(title)
                .setMessage(message)
                .setPositiveButton(posText  , null)
                .setNegativeButton(android.R.string.no, null)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
    }

    @SuppressLint("CheckResult")
    private User checkValidUser(String usernameText) {
        User user = null;
        try {user = db.userDao().getUser(usernameText).blockingGet();
        }
        catch (EmptyResultSetException err){

        }

        //returns true if the user is in the database, false if the user is not in the database
        return user;

    }

    private void launchBluetoothActivity() {
        Intent myIntent = new Intent(LoginActivity.this, BluetoothActivity.class);
        myIntent.putExtra("user", username.getText().toString()); //Optional parameters
        LoginActivity.this.startActivity(myIntent);
    }

    private void addUser() {
        Fragment createUser = LoginFragment.newInstance();
        FragmentManager fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().add(R.id.create_user_fragment_container_view,createUser,"LOGIN_FRAGMENT").commit();
        // commit add user fragment
    }
}