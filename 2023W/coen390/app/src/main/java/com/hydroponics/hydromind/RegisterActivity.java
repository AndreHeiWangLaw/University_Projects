package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

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
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseAuthWeakPasswordException;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.UserProfileChangeRequest;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class RegisterActivity extends AppCompatActivity {

    private EditText fullName, email, pwd, confirmPwd;
    private ProgressBar progressBar;
    private static final String TAG = "RegisterActivity";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        getSupportActionBar().setTitle("Register");

        Toast.makeText(getApplicationContext(), "You can register now!", Toast.LENGTH_LONG).show();

        fullName = findViewById(R.id.fullName_editText);
        email = findViewById(R.id.email_editText);
        pwd = findViewById(R.id.password_editText);
        confirmPwd = findViewById(R.id.confirmPassword_editText);
        progressBar = findViewById(R.id.progressBar);

        Button registerButton = findViewById(R.id.registerButton);
        registerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String textFullName = fullName.getText().toString();
                String textEmail = email.getText().toString();
                String textPassword = pwd.getText().toString();
                String textConfirmPwd = confirmPwd.getText().toString();

                if (TextUtils.isEmpty(textFullName)) {
                    Toast.makeText(RegisterActivity.this, "Please enter your Full Name.", Toast.LENGTH_LONG).show();
                    fullName.setError("Full Name is required (*)");
                    fullName.requestFocus();
                } else if (TextUtils.isEmpty(textEmail)) {
                    Toast.makeText(RegisterActivity.this, "Please enter your email.", Toast.LENGTH_LONG).show();
                    email.setError("Email is required (*)");
                    email.requestFocus();
                } else if (!Patterns.EMAIL_ADDRESS.matcher(textEmail).matches()) { //to validate email
                    Toast.makeText(RegisterActivity.this, "Please re-enter your email.", Toast.LENGTH_LONG).show();
                    email.setError("Valid Email is required!");
                    email.requestFocus();
                } else if (TextUtils.isEmpty(textPassword)) {
                    Toast.makeText(RegisterActivity.this, "Please enter a Password.", Toast.LENGTH_LONG).show();
                    pwd.setError("Password is required (*)");
                    pwd.requestFocus();
                } else if (textPassword.length() < 6) {
                    Toast.makeText(RegisterActivity.this, "Password should be at least 6 digits.", Toast.LENGTH_LONG).show();
                    pwd.setError("Password too weak!");
                    pwd.requestFocus();
                } else if (TextUtils.isEmpty(textConfirmPwd)) {
                    Toast.makeText(RegisterActivity.this, "Please confirm your Password.", Toast.LENGTH_LONG).show();
                    confirmPwd.setError("Password Confirmation is required (*)");
                    confirmPwd.requestFocus();
                } else if (!textPassword.equals(textConfirmPwd)) {
                    Toast.makeText(RegisterActivity.this, "Please re-enter Confirmation of Password.", Toast.LENGTH_LONG).show();
                    confirmPwd.setError("Similar Password Confirmation is required (*)");
                    confirmPwd.requestFocus();
                    pwd.clearComposingText();
                    confirmPwd.clearComposingText();
                } else {
                    progressBar.setVisibility(View.VISIBLE);
                    registerUser(textFullName,textEmail,textPassword);
                }
            }
        });
    }

    private void registerUser(String textFullName, String textEmail, String textPassword) {
        FirebaseAuth auth = FirebaseAuth.getInstance();
        auth.createUserWithEmailAndPassword(textEmail,textPassword).addOnCompleteListener(RegisterActivity.this, new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if (task.isSuccessful()){

                    FirebaseUser firebaseUser = auth.getCurrentUser();

                    UserProfileChangeRequest profileChangeRequest = new UserProfileChangeRequest.Builder().setDisplayName(textFullName).build();
                    firebaseUser.updateProfile(profileChangeRequest);

                    DatabaseReference referenceProfile = FirebaseDatabase.getInstance().getReference("Registered Users");

                    referenceProfile.child(firebaseUser.getUid()).setValue(firebaseUser.getEmail()).addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(@NonNull Task<Void> task) {

                            if (task.isSuccessful()){

                                firebaseUser.sendEmailVerification();
                                Toast.makeText(RegisterActivity.this, "User registered successfully. Please verify your email!", Toast.LENGTH_LONG).show();

                                Intent intent = new Intent(RegisterActivity.this, MainActivity.class);
                                intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
                                startActivity(intent);
                                finish();

                            }else {
                                Toast.makeText(RegisterActivity.this, "User Registration Failed. Please try again.", Toast.LENGTH_LONG).show();
                                progressBar.setVisibility(View.GONE);
                            }
                        }
                    });
                } else {
                    try {
                        throw task.getException();
                    } catch (FirebaseAuthWeakPasswordException e) {
                        pwd.setError("Your password is too weak! Please use a mix of alphabets, numbers and special characters");
                        pwd.requestFocus();
                    } catch (FirebaseAuthInvalidCredentialsException e) {
                        email.setError("Your email is invalid or already is use! Please re-enter your email.");
                        email.requestFocus();
                    }catch (FirebaseAuthUserCollisionException e){
                        email.setError("Your email is already is use! Please re-enter your email.");
                        email.requestFocus();
                    } catch (Exception e){
                        Log.e(TAG,e.getMessage());
                        Toast.makeText(RegisterActivity.this, e.getMessage(), Toast.LENGTH_LONG).show();

                    }

                }
                progressBar.setVisibility(View.GONE);
            }
        });
    }
}