package com.pothole_protector;

import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.room.EmptyResultSetException;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.pothole_protector.database.AppDatabase;
import com.pothole_protector.database.user.User;

import java.util.Objects;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link LoginFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class LoginFragment extends Fragment {
    EditText username;
    EditText password;
    Button register;

    AppDatabase database;

    public LoginFragment() {
        // Required empty public constructor
    }

    public static LoginFragment newInstance() {
        LoginFragment fragment = new LoginFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        database = AppDatabase.getDatabase(requireActivity());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_login, container, false);
        //connect ui elements to fragment
        username = view.findViewById(R.id.usernameReg);
        password = view.findViewById(R.id.passwordReg);
        register = view.findViewById(R.id.reg);
        register.setOnClickListener(l->regUser());

        // Inflate the layout for this fragment
        return view;

    }

    private void regUser() {
        if(username.getText().toString().isEmpty() || password.getText().toString().isEmpty()){
            LoginActivity.showAlert("Username or Password invalid", "Please type in a username and password", "Okay", null, requireActivity());
        }
        else{
            // set up a user in database
            String setUsername = username.getText().toString();
            String setPassword = password.getText().toString();

            //check for duplicates
            if(checkUserExists(setUsername)){
                LoginActivity.showAlert("Username already in use", "Please make a different username", "Okay", null, requireActivity());
                return;
            }


            // send user to database
            new Thread(() -> {
                User newUser = new User(setUsername, setPassword);
                database.userDao().insertUser(newUser);
            }).start();

            // get rid of fragment
            getActivity().onBackPressed();
        }
    }

    private boolean checkUserExists(String usernameText) {
        User user = null;
        try {
            user = database.userDao().getUser(usernameText).blockingGet();
        }
        catch (EmptyResultSetException err){

        }

        //returns true if the user is in the database, false if the user is not in the database
        return user != null;
    }
}