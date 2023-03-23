package com.pothole_protector;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.pothole_protector.database.AppDatabase;
import com.pothole_protector.database.user.User;

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
        database = AppDatabase.getDatabase(getActivity());
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
            // throw up an alert dialog
        }
        else{
            // set up a user in database
            String setUsername = username.getText().toString();
            String setPassword = password.getText().toString();

            //check for duplicates



            // send user to database
            new Thread(() -> {
                User newUser = new User(setUsername, setPassword);
                database.userDao().insertUser(newUser);
            }).start();



            // get rid of fragment


        }
    }
}