import React from 'react';
import SidebarView from './SidebarView';

const SidebarController = ({ show, isDarkMode}) => {
    const sidebarClass = show ? "" : "sidebar-collapsed";
    return <SidebarView sidebarClass={sidebarClass} isDarkMode={isDarkMode} />;
};

export default SidebarController;
