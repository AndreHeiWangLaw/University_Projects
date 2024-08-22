package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthInvalidCredentialsException;
import com.google.firebase.auth.FirebaseAuthInvalidUserException;
import com.google.firebase.auth.FirebaseUser;

public class LoginActivity extends AppCompatActivity {

    private EditText email, pwd;
    private ProgressBar progressBar;
    private FirebaseAuth authProfile;
    private static final String TAG = "LoginActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        getSupportActionBar().setTitle("Login");

        email = findViewById(R.id.email_editText);
        pwd = findViewById(R.id.pwd_editText);
        progressBar = findViewById(R.id.progressBar);

        authProfile = FirebaseAuth.getInstance();

        Button loginButton = findViewById(R.id.loginButton);

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String emailText = email.getText().toString();
                String pwdText = pwd.getText().toString();

                if (TextUtils.isEmpty(emailText)) {
                    Toast.makeText(LoginActivity.this, "Please enter email.", Toast.LENGTH_SHORT).show();
                    email.setError("Email is required (*)");
                    email.requestFocus();
                } else if(!Patterns.EMAIL_ADDRESS.matcher(emailText).matches()){
                    Toast.makeText(LoginActivity.this, "Please re-enter a valid email.", Toast.LENGTH_SHORT).show();
                    email.setError("Valid Email required!");
                    email.requestFocus();
                } else if (TextUtils.isEmpty(pwdText)) {
                    Toast.makeText(LoginActivity.this, "Please enter password.", Toast.LENGTH_SHORT).show();
                    pwd.setError("Password Required (*)");
                    pwd.requestFocus();
                } else {
                    progressBar.setVisibility(View.VISIBLE);
                    loginUser(emailText,pwdText);
                }
            }
        });
    }

    private void loginUser(String emailText, String pwdText) {
        authProfile.signInWithEmailAndPassword(emailText, pwdText).addOnCompleteListener(LoginActivity.this, new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if (task.isSuccessful()){
                    Toast.makeText(LoginActivity.this, "You are now Logged In!", Toast.LENGTH_SHORT).show();

                    FirebaseUser firebaseUser = authProfile.getCurrentUser();

                    if (firebaseUser.isEmailVerified()){
                        Toast.makeText(LoginActivity.this, "You are now logged in!", Toast.LENGTH_SHORT).show();
                        startActivity(new Intent(LoginActivity.this, Home.class));
                        finish();

                    } else {
                        firebaseUser.sendEmailVerification();
                        authProfile.signOut();
                        showAlertDialog();
                    }
                } else {
                    try {
                        throw task.getException();
                    }catch(FirebaseAuthInvalidUserException e){
                        email.setError("User does not exist or is no longer existing. Please register again.");
                        email.requestFocus();
                    } catch (FirebaseAuthInvalidCredentialsException e){
                        email.setError("Invalid Input. Please try again.");
                        email.requestFocus();
                    } catch (Exception e){
                        Log.e(TAG, e.getMessage());
                        Toast.makeText(LoginActivity.this, e.getMessage(), Toast.LENGTH_LONG).show();
                    }
                    Toast.makeText(LoginActivity.this, "Something went wrong! try again.", Toast.LENGTH_SHORT).show();
                }
                progressBar.setVisibility(View.GONE);
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        if (authProfile.getCurrentUser() != null){
            Toast.makeText(this, "Already Logged In! Welcome Back.", Toast.LENGTH_SHORT).show();
            startActivity(new Intent(LoginActivity.this, Home.class));
            finish();
        } else {
            Toast.makeText(this, "You can Login now!", Toast.LENGTH_SHORT).show();
        }
    }

    private void showAlertDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(LoginActivity.this);
        builder.setTitle("Email Not Verified");
        builder.setMessage("Please verify your email in order to access app. You cannot login without email verification.");

        builder.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                Intent intent = new Intent(Intent.ACTION_MAIN);
                intent.addCategory(Intent.CATEGORY_APP_EMAIL);
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
            }
        });
//tear
        AlertDialog alertDialog = builder.create();
        alertDialog.show();

    }
}