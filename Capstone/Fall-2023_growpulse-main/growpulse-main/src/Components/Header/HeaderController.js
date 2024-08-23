import React, {useEffect, useState} from 'react';
import {useNavigate} from 'react-router-dom';
import {auth, database} from '../../firebaseConfig';
import HeaderView from './HeaderView';
import { ref, get } from 'firebase/database';

const HeaderController = ({ toggleSidebar, sidebarExpanded, toggleTheme, isDarkMode }) => {

    const currentUser = auth.currentUser;
    const [fullName, setFullName] = useState('');
    const navigate = useNavigate();
    const handleLogout = async () => {
        try {
            await auth.signOut();
            console.log('Logged out successfully');
            navigate('/');
        } catch (error) {
            console.error('Error logging out:', error.message);
        }
    };

    useEffect(() => {
        const fetchFullName = async () => {
            try {
                if (currentUser) {
                    await currentUser.reload(); // Refresh user properties

                    // Use ref to create a reference to the database path
                    const userRef = ref(database, `Registered Users/${currentUser.uid}`);

                    // Fetching the data from the database reference
                    const snapshot = await get(userRef);

                    if (snapshot.exists()) {
                        const userData = snapshot.val();
                        const fullName = userData.fullName; // Ensure your database has a 'fullName' field
                        setFullName(fullName); // Update state with the fetched full name
                    } else {
                        console.log("User data not found"); // Handle the case where no user data exists
                    }
                }
            } catch (error) {
                console.error('Error fetching full name:', error.message); // Handling errors
            }
        };

        fetchFullName();
    }, [currentUser]); // Dependency array includes currentUser to refetch when currentUser changes


    return (
        <HeaderView
            toggleSidebar={toggleSidebar}
            sidebarExpanded={sidebarExpanded}
            toggleTheme={toggleTheme}
            isDarkMode={isDarkMode}
            fullName={fullName}
            handleLogout={handleLogout}
        />
    );
};

export default HeaderController;
