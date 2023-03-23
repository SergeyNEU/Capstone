package com.pothole_protector;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.room.Room;

import android.app.FragmentTransaction;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.pothole_protector.database.AppDatabase;

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




    }

    private void logInUser() {
        String usernameText = username.getText().toString();
        String passwordText = password.getText().toString();
        // check database for a matching username and password




        // if not found, throw up a dialog




        // else launch the next activityq
    }

    private void addUser() {
        Fragment createUser = LoginFragment.newInstance();
        FragmentManager fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().replace(R.id.create_user_fragment_container_view,createUser).commit();
        // commit add user fragment
    }
}