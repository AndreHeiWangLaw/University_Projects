import React from 'react';
import './Schedule.css';

const Schedule = ({ sidebarExpanded }) => {

    return (
        <div className={`background-overlay ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'}`}>
            <div className="schedule">
                
            </div>
        </div>
    );
};

export default Schedule;
