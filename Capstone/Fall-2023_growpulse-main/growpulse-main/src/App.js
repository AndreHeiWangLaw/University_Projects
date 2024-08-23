import React, { useState, useEffect } from 'react';
import {BrowserRouter as Router, Navigate, Route, Routes} from 'react-router-dom';
import { auth } from './firebaseConfig';
import LandingPageView from './Components/LandingPage/LandingPageView';
import LoginModal from './Components/LoginModal/LoginModal';
import SidebarController from './Components/SideBar/SidebarController';
import TopbarController from './Components/Header/HeaderController';
import AllSystems from "./Components/AllSystems/AllSystems";
import Overview from "./Components/Overview/Overview";
import SystemSettings from './Components/SystemSettings/SystemSettings';
import SystemAlerts from "./Components/SystemAlerts/systemAlerts";
import Settings from "./Components/Settings/Settings";
import Help from "./Components/Help/Help";
import Schedule from "./Components/Schedule/Schedule";

const App = () => {
    const [isLoggedIn, setIsLoggedIn] = useState(false);
    const [isEmailVerified, setIsEmailVerified] = useState(false);
    const [sidebarExpanded, setSidebarExpanded] = useState(true);
    const [isDarkMode, setIsDarkMode] = useState(false);

    useEffect(() => {
        const unsubscribe = auth.onAuthStateChanged((user) => {
            if (user) {
                setIsLoggedIn(true);
                setIsEmailVerified(user.emailVerified);
            } else {
                setIsLoggedIn(false);
                setIsEmailVerified(false);
            }
        });

        return () => unsubscribe();
    }, []);

    return (
        <Router>
            <div className="App">
            {isLoggedIn && isEmailVerified && <SidebarController show={sidebarExpanded} isDarkMode={isDarkMode} />}
                {isLoggedIn && isEmailVerified && <TopbarController 
                    toggleSidebar={() => setSidebarExpanded(prev => !prev)}
                    toggleTheme={() => setIsDarkMode(prev => !prev)} 
                    sidebarExpanded={sidebarExpanded}
                    isDarkMode={isDarkMode}
                />}
                {isLoggedIn && isEmailVerified ? (
                    <Routes>
                        <Route path="/" element={<Navigate to="/allSystems" />} />
                        <Route path="/allSystems" element={<AllSystems key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/Overview/:systemName" element={<Overview key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/systemSettings/:systemName" element={<SystemSettings key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/schedule/:systemName" element={<Schedule key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/systemAlerts" element={<SystemAlerts key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/settings" element={<Settings key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                        <Route path="/help" element={<Help key={window.location.pathname} sidebarExpanded={sidebarExpanded} isDarkMode={isDarkMode}/>} />
                    </Routes>
                ) : (
                    <Routes>
                        <Route path="/" element={<LandingPageView />} />
                        {isLoggedIn ? (
                            <Route path="/allSystems" element={<AllSystems />} />
                        ) : (
                            <Route path="/login" element={<LoginModal />} />
                        )}
                    </Routes>
                )}
            </div>
        </Router>
    );
};

export default App;
