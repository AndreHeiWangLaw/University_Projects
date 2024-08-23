import React, { useState } from 'react';
import Modal from 'react-modal';
import { auth } from '../../firebaseConfig';
import './LoginModal.css';
import { ToastContainer, toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import {useNavigate} from "react-router-dom";
import { signInWithEmailAndPassword } from 'firebase/auth';


Modal.setAppElement('#root');

const LoginModal = ({ isOpen, onRequestClose }) => {
    const navigate = useNavigate();
    const [formData, setFormData] = useState({
        email: '',
        password: '',
    });

    const [errorMessage, setErrorMessage] = useState('');

    const handleChange = (e) => {
        setFormData({ ...formData, [e.target.name]: e.target.value });
    };

    const showSuccessMessage = () => {
        toast.success('You are logged in!', {
            position: 'top-center',
            autoClose: 5000,
            closeOnClick: true,
            draggable: true,
            progress: undefined,
            style: {
                fontFamily: 'Poppins, sans-serif',
            },
        });
    };


    const handleSubmit = async (e) => {
        e.preventDefault();
    
        try {
            // Use the signInWithEmailAndPassword method from firebase/auth
            await signInWithEmailAndPassword(auth, formData.email, formData.password);
            console.log('Logged in successfully');
            showSuccessMessage();
            onRequestClose();
            navigate('/allSystems');
        } catch (error) {
            console.error('Error logging in:', error.message);
            setErrorMessage(error.message);
        }
    };


    return (
        <Modal
            isOpen={isOpen}
            onRequestClose={onRequestClose}
            className="login-modal"
            overlayClassName="login-modal-overlay"
        >
            <h2>Login</h2>
            {errorMessage && <p className="error-message">{errorMessage}</p>}
            <div className="login-form-container">
                <form onSubmit={handleSubmit} className="login-form">
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
                    <ToastContainer />
                    <button type="submit" className="login-submit">Login</button>
                </form>
            </div>
        </Modal>
    );
};

export default LoginModal;
