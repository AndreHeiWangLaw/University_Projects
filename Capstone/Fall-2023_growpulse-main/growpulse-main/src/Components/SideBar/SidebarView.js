import React, { useState, useEffect } from 'react';
import { Link, useLocation  } from 'react-router-dom';
import './Sidebar.css';
import GrowPulseLogo from '../Images/GrowpulseLogos/growpulse-high-resolution-logo-transparent.png';
import systemIcon from '../Images/SidebarIcons/system.png';
import expand from '../Images/SidebarIcons/down.png';
import collapse from '../Images/SidebarIcons/up.png';
import { database, auth } from '../../firebaseConfig';
import overviewIcon from '../Images/SidebarIcons/overview.png';
import notificationIcon from '../Images/HeaderIcons/notification.png';
import settingsIcon from '../Images/SidebarIcons/settings.png';
import supportIcon from '../Images/SidebarIcons/support.png';
import trashIcon from '../Images/SidebarIcons/trash.png';
import { addNewSystem } from './addNewSystem';
import { ref, remove, get } from 'firebase/database';

const SidebarView = ({ sidebarClass, isDarkMode}) => {
    const [expandedSystem, setExpandedSystem] = useState(null);
    const [systems, setSystems] = useState([]);
    const [activeLink, setActiveLink] = useState(null);
    const location = useLocation();

    const handleAddNewSystem = async () => {
        const currentUser = auth.currentUser;
        if (currentUser) {
            try {
                const newSystemId = await addNewSystem(currentUser.uid);
                setSystems([...systems, newSystemId]);
            } catch (error) {
                console.error(error);
            }
        }
    };

    useEffect(() => {
        const fetchSystems = async () => {
            const currentUser = auth.currentUser;
            if (currentUser) {
                // Using ref to create a reference to the database path
                const userRef = ref(database, `Registered Users/${currentUser.uid}`);

                try {
                    // Fetching the data from the database reference
                    const snapshot = await get(userRef);
                    if (snapshot.exists()) {
                        const fetchedSystems = [];
                        snapshot.forEach(childSnapshot => {
                            const systemName = childSnapshot.key;
                            if (systemName.startsWith("System")) {
                                fetchedSystems.push(systemName);
                            }
                        });
                        setSystems(fetchedSystems); // Update the state with the fetched systems
                    } else {
                        console.log("No systems available"); // Handle the case where no data exists
                    }
                } catch (error) {
                    console.error("Error fetching systems:", error); // Handling errors
                }
            }
        };

        fetchSystems(); // Call the async function to fetch systems
    }, []);

    useEffect(() => {
        const currentPath = location.pathname;
        
        if (currentPath.includes('settings')) {
            setActiveLink('settings');
        }  else if (currentPath.includes('allSystems')){
            setActiveLink('allSystems');
        } 
    }, [location, systems]);

    const handleSystemToggle = (system) => {
        if (expandedSystem === system) {
            setExpandedSystem(null);
        } else {
            setExpandedSystem(system);
        }
    };

    const handleLinkClick = (linkName) => {

        setActiveLink(linkName);
    };

    const isPageOfSystemActive = (system) => {
        // Ensure activeLink is a string before calling includes
        return activeLink && activeLink.includes(system);
    };
    
    const handleDeleteSystem = async (systemId) => {
        const confirmed = window.confirm(`Are you sure you want to delete ${systemId}?`);
        if (confirmed) {
            const currentUser = auth.currentUser;
            if (currentUser) {
                try {
                    const systemRef = ref(database, `Registered Users/${currentUser.uid}/${systemId}`);
                    await remove(systemRef);
                    // Update local state
                    setSystems(systems.filter(system => system !== systemId));
                } catch (error) {
                    console.error("Error deleting system:", error);
                }
            }
        }
    };

    return (
        <div className={`sidebar ${sidebarClass} ${isDarkMode ? 'dark-mode' : ''}`}>
            <img src={GrowPulseLogo} alt="Header Icon" className="sidebar-icon"/>
            <ul>
                <li>
                    <Link   
                        to="/allSystems" 
                        className={`Overview-link ${activeLink === "allSystems" ? "active" : ""}`} 
                        onClick={() => handleLinkClick("allSystems")}
                    >
                        <h2>Account Overview</h2>
                        <div className="Overview">
                            <div className={`icon-container ${activeLink === "allSystems" ? "highlight" : ""} all-systems-icon`}>
                                <img src={overviewIcon} alt="overview Icon" className="subtitle-icon"/>
                            </div>
                            <div className="h3-container">
                                    <h3>All Systems</h3>
                            </div>
                        </div>
                    </Link>
                </li>
                <li>
                    <div className="addSystem">
                        <h2>Individual Systems
                            <span className="systems-number">{systems.length}</span>
                            <button onClick={handleAddNewSystem} className="add-system-button">+</button>
                        </h2>
                    </div>
                    {systems.map((system, index) => {
                        const systemLetter = system.charAt(6);
                        return (
                            <div key={index} className="system-container">
                                <div className="Dashboards" onClick={() => handleSystemToggle(system)}>
                                    <div className={`icon-container ${isPageOfSystemActive(system) ? "highlight" : ""}`}>
                                        <img src={systemIcon} alt="system Icon" className="system-icon"/>
                                        <span className="system-letter">{systemLetter}</span>
                                    </div>
                                    <div className="h3-container">
                                        <h3>{system}</h3>
                                    </div>
                                    <div className="icon-actions">
                                        <img src={trashIcon} alt="Trash Icon" className="trash" onClick={(e) => { e.stopPropagation(); handleDeleteSystem(system); }}/>
                                        <img src={expandedSystem === system ? collapse : expand} alt="Expand Icon" className="expand"/>
                                    </div>
                                </div>
                                {expandedSystem === system && (
                                    <div className="bullet-points">
                                        <Link
                                            to={`/Overview/${system}`}
                                            onClick={() => handleLinkClick(`Overview-${system}`)}
                                            className={activeLink === `Overview-${system}` ? "active" : ""}>
                                            • Overview
                                        </Link>
                                        <Link
                                            to={`/systemSettings/${system}`}
                                            onClick={() => handleLinkClick(`SystemSettings-${system}`)}
                                            className={activeLink === `SystemSettings-${system}` ? "active" : ""}>
                                            • System Settings
                                        </Link>
                                        {/*<Link
                                            to={`/schedule/${system}`}
                                            onClick={() => handleLinkClick(`Schedule-${system}`)}
                                            className={activeLink === `Schedule-${system}` ? "active" : ""}>
                                            • Schedule
                                        </Link>*/}
                                    </div>
                                )}
                            </div>
                        );
                    })}
                </li>
                <li>
                    <Link 
                        to="/systemAlerts"  
                        className={`systemAlerts-link ${activeLink === "systemAlerts" ? "active" : ""}`} 
                        onClick={() => handleLinkClick(`systemAlerts`)}>
                            <h2>Alerts</h2>
                            <div className="Overview">
                                <div className={`icon-container ${activeLink === "systemAlerts" ? "highlight" : ""}`}>
                                    <img src={notificationIcon} alt="notification Icon" className="subtitle-icon"/>
                                </div>
                                <div className="h3-container">
                                    <h3>System Alerts</h3>
                                </div>
                            </div>
                    </Link>
                </li>
                <li>
                    <Link 
                        to="/settings"  
                        className={`settings-link ${activeLink === "settings" ? "active" : ""}`} 
                        onClick={() => handleLinkClick(`settings`)}>
                        <h2>Settings</h2>
                        <div className="Overview">
                            <div className={`icon-container ${activeLink === "settings" ? "highlight" : ""}`}>
                                <img src={settingsIcon} alt="settings Icon" className="subtitle-icon"/>
                            </div>
                            <div className="h3-container">
                                <h3>Account Settings</h3>
                            </div>
                        </div>
                    </Link>
                </li>
                <li>
                    <Link 
                        to="/help"  
                        className={`help-link ${activeLink === "help" ? "active" : ""}`} 
                        onClick={() => handleLinkClick(`help`)}>
                        <h2>Support</h2>
                        <div className="Overview">
                            <div className={`icon-container ${activeLink === "help" ? "highlight" : ""}`}>
                                <img src={supportIcon} alt="support Icon" className="subtitle-icon"/>
                            </div>
                            <div className="h3-container">
                                <h3>Help</h3>
                            </div>
                        </div>
                    </Link>
                </li>
            </ul>
        </div>
    );
};

export default SidebarView;
