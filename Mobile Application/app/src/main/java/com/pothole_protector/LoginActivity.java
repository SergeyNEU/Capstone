package com.pothole_protector;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.pothole_protector.database.AppDatabase;
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

    private void logInUser() {
        String usernameText = username.getText().toString();
        String passwordText = password.getText().toString();


        // check database for a matching username and password
        db.userDao().getUser(usernameText).subscribeOn(Schedulers.newThread()).subscribe(fetchedUser -> {
            if(!fetchedUser.password.equals(passwordText)){
                // throw up a dialog saying saying wrong password and shit
                return;
            }
            launchBluetoothActivity();
        });


    }

    private void launchBluetoothActivity() {
        Intent myIntent = new Intent(LoginActivity.this, BluetoothActivity.class);
        myIntent.putExtra("user", username.getText().toString()); //Optional parameters
        LoginActivity.this.startActivity(myIntent);
    }

    private void addUser() {
        Fragment createUser = LoginFragment.newInstance();
        FragmentManager fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().replace(R.id.create_user_fragment_container_view,createUser).commit();
        // commit add user fragment
    }
}