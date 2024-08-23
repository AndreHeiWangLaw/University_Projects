import React, { useState } from 'react';
import Modal from 'react-modal';
import { getAuth, createUserWithEmailAndPassword, sendEmailVerification } from "firebase/auth";
import { getDatabase, ref, set } from "firebase/database";
import './RegisterModal.css';
import toast from 'react-hot-toast';
import LoginModal from '../LoginModal/LoginModal';

Modal.setAppElement('#root');

const RegisterModal = ({ isOpen, onRequestClose }) => {
    const [formData, setFormData] = useState({
        fullName: '',
        email: '',
        password: '',
        confirmPassword: '',
    });

    const [errorMessage, setErrorMessage] = useState('');
    const [isLoginModalOpen, setIsLoginModalOpen] = useState(false);

    // Initialize Firebase Auth and Database
    const auth = getAuth();
    const database = getDatabase();

    const handleChange = (e) => {
        setFormData({ ...formData, [e.target.name]: e.target.value });
    };

    const handleSignInClick = () => {
        onRequestClose(); // Close RegisterModal
        setIsLoginModalOpen(true); // Open LoginModal
    };

    const isValidPassword = (password) => {
        const regex = /^(?=.*\d)(?=.*[a-zA-Z]).{6,}$/;
        return regex.test(password);
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        if (formData.password !== formData.confirmPassword) {
            setErrorMessage('Passwords do not match.');
            return;
        }

        if (!isValidPassword(formData.password)) {
            setErrorMessage('Password must be at least 6 characters and contain 2 numbers.');
            return;
        }

        try {
            const userCredential = await createUserWithEmailAndPassword(
              auth,
              formData.email,
              formData.password
            );
            console.log('Account created successfully:', userCredential.user);
        
            
            await sendEmailVerification(userCredential.user);      

            // Get the user's UID
            const uid = userCredential.user.uid;

            // Create a new user in Firebase Realtime Database
            await set(ref(database, `Registered Users/${uid}`), {
                fullName: formData.fullName,
                email: formData.email,
            });

            // Show a success toast
            toast.success('Please check your email to verify your account.', {
                style: {
                    background: '#333',
                    color: '#fff',
                    fontFamily: 'Poppins',
                },
            });

            // Delay the redirect to give the toast time to display
            setTimeout(() => {
                onRequestClose();
            }, 5000); // Delay for 5 seconds

        } catch (error) {
            console.error('Error creating account:', error.message);
            setErrorMessage(error.message);
        }
    };

    return (
        <>

<Modal
                isOpen={isOpen}
                onRequestClose={onRequestClose}
                className="register-modal"
                overlayClassName="register-modal-overlay"
            >
                <h2>Register</h2>
                {errorMessage && <p className="error-message">{errorMessage}</p>}
                <div className="register-form-container">
                    <form onSubmit={handleSubmit} className="register-form">
                        <label htmlFor="fullName">Full Name</label>
                        <input
                            type="text"
                            id="fullName"
                            name="fullName"
                            value={formData.fullName}
                            onChange={handleChange}
                            required
                        />
                        <label htmlFor="email">Email</label>
                        <input
                            type="email"
                            id="email"
                            name="email"
                            value={formData.email}
                            onChange={handleChange}
                            required
                        />
                        <label htmlFor="password">Password</label>
                        <input
                            type="password"
                            id="password"
                            name="password"
                            value={formData.password}
                            onChange={handleChange}
                            required
                        />
                        <label htmlFor="confirmPassword">Confirm Password</label>
                        <input
                            type="password"
                            id="confirmPassword"
                            name="confirmPassword"
                            value={formData.confirmPassword}
                            onChange={handleChange}
                            required
                        />
                        <button type="submit" className="register-submit">Register</button>
                        <p className="account-available">Already have an account?
                            <span onClick={handleSignInClick}>Sign In</span>
                        </p>
                    </form>
                </div>
            </Modal>
            <LoginModal isOpen={isLoginModalOpen} onRequestClose={() => setIsLoginModalOpen(false)} />
        </>
    );
};

export default RegisterModal;
