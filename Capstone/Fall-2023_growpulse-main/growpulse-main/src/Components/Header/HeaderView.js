import React, { useEffect, useState, useRef } from 'react';
import './Header.css';
import collapseLeft from '../Images/HeaderIcons/left_collapse.png';
import expandRight from '../Images/HeaderIcons/double-right.png';
import fullScreen from '../Images/HeaderIcons/full_screen.png';
import notificationIcon from '../Images/HeaderIcons/notification.png';
import tasks from '../Images/HeaderIcons/tasks.png';
import darkMode from '../Images/HeaderIcons/night-mode.png';
import lightMode from '../Images/HeaderIcons/sun-white.png';
import extendWhite from '../Images/HeaderIcons/extend-white.png';
import collapseWhite from '../Images/HeaderIcons/collapse-white.png';
import fullScreenWhite from '../Images/HeaderIcons/full-screen-white.png';
import notificationWhite from '../Images/HeaderIcons/notification-white.png';
import taskWhite from '../Images/HeaderIcons/task-list-white.png';
import { firestore, auth } from '../../firebaseConfig'; 
import { collection, query, where, onSnapshot } from 'firebase/firestore';
import { startOfDay, addDays } from 'date-fns';
import { doc, updateDoc } from 'firebase/firestore';


const HeaderView = ({ toggleSidebar, sidebarExpanded, toggleTheme, isDarkMode, fullName, handleLogout }) => {
    const formatDate = () => {
        const currentDate = new Date();
        const monthNames = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
        const date = currentDate.getDate();
        const month = monthNames[currentDate.getMonth()];
        let hours = currentDate.getHours();
        const minutes = String(currentDate.getMinutes()).padStart(2, '0');
        

        // Convert 24 to 00
        if (hours === 24) {
            hours = 0;
        }
        const time = `${String(hours).padStart(2, '0')}:${minutes}`;

        return ` ${month} ${date} | ${time}`;
    };

    const [currentDateTime, setCurrentDateTime] = useState(formatDate());
    const topbarClass = sidebarExpanded ? "" : "topbar-expanded";
    const [showTasksDropdown, setShowTasksDropdown] = useState(false);
    const [upcomingTasks, setUpcomingTasks] = useState([]);
    const tasksDropdownRef = useRef(null);

    const formatDateForComparison = (date) => {
        return date.toISOString().split('T')[0]; 
    };

    useEffect(() => {
        // Sets 'today' to the start of the current day.
        const today = startOfDay(new Date());
        // Sets 'fiveDaysLater' to the end of the day, 5 days from today.
        const fiveDaysLater = addDays(today, 5);
    
        const tasksRef = collection(firestore, `Registered Users/${auth.currentUser.uid}/Tasks`);
        // The Firestore query should compare date strings, ensure your 'dueDate' in Firestore is stored in 'YYYY-MM-DD' format or adjust accordingly.
        const q = query(
            tasksRef, 
            where('dueDate', '>=', formatDateForComparison(today)), 
            where('dueDate', '<=', formatDateForComparison(fiveDaysLater))
        );
    
        const unsubscribe = onSnapshot(q, (querySnapshot) => {
            const tasks = [];
            querySnapshot.forEach((doc) => {
                tasks.push({ id: doc.id, ...doc.data() });
            });
            setUpcomingTasks(tasks);
        });
    
        return () => unsubscribe();
    }, []);


    const toggleFullScreen = () => {
        if (!document.fullscreenElement) {
            if (document.documentElement.requestFullscreen) {
                document.documentElement.requestFullscreen();
            } else if (document.documentElement.mozRequestFullScreen) { /* Firefox */
                document.documentElement.mozRequestFullScreen();
            } else if (document.documentElement.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
                document.documentElement.webkitRequestFullscreen();
            } else if (document.documentElement.msRequestFullscreen) { /* IE/Edge */
                document.documentElement.msRequestFullscreen();
            }
        } else {
            if (document.exitFullscreen) {
                document.exitFullscreen();
            } else if (document.mozCancelFullScreen) { /* Firefox */
                document.mozCancelFullScreen();
            } else if (document.webkitExitFullscreen) { /* Chrome, Safari and Opera */
                document.webkitExitFullscreen();
            } else if (document.msExitFullscreen) { /* IE/Edge */
                document.msExitFullscreen();
            }
        }
    };

    useEffect(() => {
        const interval = setInterval(() => {
            setCurrentDateTime(formatDate());
        }, 1000);

        return () => {
            clearInterval(interval);
        };
    }, []);

    const toggleTaskCompleted = async (taskId, completed) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        try {
            await updateDoc(taskDocRef, { completed: !completed });
        } catch (error) {
            console.error("Error updating task completed status:", error);
        }
    };

    useEffect(() => {
        const handleClickOutside = (event) => {
            if (tasksDropdownRef.current && !tasksDropdownRef.current.contains(event.target)) {
                setShowTasksDropdown(false);
            }
        };
    
        document.addEventListener('mousedown', handleClickOutside);
    
        return () => {
            document.removeEventListener('mousedown', handleClickOutside);
        };
    }, []);
    

    return (
        <div className={`topbar ${topbarClass} ${isDarkMode ? 'dark-mode' : ''}`}>
            <div className="left-side">
                <img
                    src={sidebarExpanded ? (isDarkMode ? collapseWhite : collapseLeft) : (isDarkMode ? extendWhite : expandRight)}
                    alt={sidebarExpanded ? "Collapse" : "Expand"}
                    className="collapse-toggle"
                    onClick={toggleSidebar} />
                <h1>Hello, {fullName}</h1>
                <div className='current-date'>{currentDateTime}</div>
            </div>
            <div className="right-side">
                <img
                    src={isDarkMode ? lightMode : darkMode}
                    alt="Theme Toggle"
                    className="icons dark-mode-icon"
                    onClick={toggleTheme}
                />
                <img
                    src={isDarkMode ? taskWhite : tasks}
                    alt="Tasks"
                    className="icons tasks"
                    onClick={() => setShowTasksDropdown(!showTasksDropdown)}
                />
                {showTasksDropdown && (
                    <div className="tasks-dropdown" ref={tasksDropdownRef}>
                        <h4>Upcoming Dues:<span className='days'> (Next 5 Days Only)</span></h4>
                        {upcomingTasks.length > 0 ? (
                            upcomingTasks.map(task => (
                                <div key={task.id} className={`task-entry ${task.completed ? 'completed-task' : ''}`}>
                                    <input
                                        type="checkbox"
                                        checked={task.completed}
                                        onChange={() => toggleTaskCompleted(task.id, task.completed)}
                                    />
                                    <span className='taskName'>"{task.name}"</span> <span className='due'>is due on</span> <span className='taskDate'>{task.dueDate}</span><span className={`priority ${task.priority}`}> [{task.priority}]</span> 
                                </div>
                            ))
                        ) : (
                            <div className="no-tasks">No upcoming tasks</div>
                        )}
                    </div>
                )}
                <img src={isDarkMode ? notificationWhite : notificationIcon} alt="Notifications" className="icons" />
                <img src={isDarkMode ? fullScreenWhite : fullScreen} alt="Full Screen" className="icons" onClick={toggleFullScreen} />
                <button className="logout-button" onClick={handleLogout}>Logout</button>
            </div>
        </div>
    );
};

export default HeaderView;
