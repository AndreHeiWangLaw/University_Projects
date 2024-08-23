import React, { useState } from 'react';
import './Help.css';
// You may need to import a method to send emails or use an API here

const Help = ({ sidebarExpanded, isDarkMode }) => {
    const [email, setEmail] = useState('');
    const [subject, setSubject] = useState('');
    const [description, setDescription] = useState('');
    const [message, setMessage] = useState('');
    const [messageType, setMessageType] = useState('');

    const handleSubmit = async (event) => {
        event.preventDefault();
        setMessage(''); // Reset message
        setMessageType('');

        try {
            const response = await fetch('/send-email', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ email, subject, description }),
            });            

            if (response.ok) {
                setMessage('Email sent successfully!');
                setMessageType('success');
                setEmail('');
                setSubject('');
                setDescription('');
            } else {
                setMessage('Failed to send email.');
                setMessageType('error');
            }
        } catch (error) {
            console.error("Error sending email: ", error);
            setMessage('An error occurred.');
            setMessageType('error');
        }
    };

    return (
        <div className={`background-overlay ${isDarkMode ? 'dark-mode' : ''} ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'}`}>
            <div className={`help ${isDarkMode ? 'dark-mode' : ''}`}>
                {message && <div className={`message ${messageType}`}>{message}</div>}
                <form onSubmit={handleSubmit}>
                    <div className="form-group">
                        <label htmlFor="email">Email:</label>
                        <input
                            type="email"
                            id="email"
                            value={email}
                            onChange={(e) => setEmail(e.target.value)}
                            required
                        />
                    </div>
                    <div className="form-group">
                        <label htmlFor="subject">Subject:</label>
                        <input
                            type="text"
                            id="subject"
                            value={subject}
                            onChange={(e) => setSubject(e.target.value)}
                            required
                        />
                    </div>
                    <div className="form-group">
                        <label htmlFor="description">Description:</label>
                        <textarea
                            id="description"
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                            required
                        />
                    </div>
                    <div className='submit-button'>
                        <button type="submit">Submit</button>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default Help;
