import React, { useState, useEffect } from 'react';
import './systemAlerts.css';
import on from '../Images/Dashboard/ON.png';
import off_icon from '../Images/Dashboard/OFF.png';
import settingsIcon from '../Images/SidebarIcons/settings.png';
import { auth, database } from "../../firebaseConfig";
import { ref, onValue } from "firebase/database";
import {
  toggleAirTempAlert, toggleCamAlert, toggleDpAlert,
  toggleHumidityAlert,
  togglePhAlert,
  toggleTdsAlert,
  toggleWaterTempAlert
} from '../Services/AlertsServices';
import scroll from '../Images/HeaderIcons/scroll-grey.png';
import AlertsSettingsModal from '../AlertsSettingsModal/AlertsSettingsModal';
import off_white from '../Images/Dashboard/ON-white.png';

const SystemAlerts = ({ sidebarExpanded, isDarkMode }) => {
  const [systems, setSystems] = useState([]);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [selectedSystem, setSelectedSystem] = useState(null);

  const openModal = (systemName) => {
    setSelectedSystem(systemName);
    setIsModalOpen(true);
  };

  const scrollToRight = () => {
    window.scrollTo({
      left: document.body.scrollWidth,
      behavior: 'smooth'
    });
  };

  useEffect(() => {
    const fetchSystems = () => {
      const currentUser = auth.currentUser;
      if (currentUser) {
        const systemsRef = ref(database, `Registered Users/${currentUser.uid}`);
        onValue(systemsRef, (snapshot) => {
          const systemsData = [];
          snapshot.forEach((childSnapshot) => {
            if (childSnapshot.key.includes('System')) {
              const systemName = childSnapshot.key;
              const alerts = childSnapshot.val().Alerts || {};
              systemsData.push({
                systemName,
                alerts
              });
            }
          });
          setSystems(systemsData);
        });
      }
    };

    fetchSystems();
  }, []);

  // Function to safely access nested properties
  const safelyGetNestedProperty = (object, ...keys) => {
    return keys.reduce((obj, key) => (obj && obj[key] != null) ? obj[key] : null, object);
  };

  // Function to format number or return default value
  const formatNumber = (number, defaultValue = '-') => {
    const num = parseFloat(number);
    return isNaN(num) ? defaultValue : num.toFixed(2);
  };

  return (
    <div className={`background-overlay ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'} ${isDarkMode ? 'dark-mode' : ''}`}>
      <div className={`system-alerts ${isDarkMode ? 'dark-mode' : ''}`}>
        <table className="system-table alerts-table">
          <thead>
            <tr>
            <th>System</th>
              <th>pH</th>
              <th>TDS</th>
              <th>Water Temp.</th>
              <th>Humidity</th>
              <th>Air Temp.</th>
              <th>Dosing Pumps</th>
              <th>CAM</th>
            </tr>
          </thead>
          <tbody>
            {systems.map((system) => {
              const phMin = formatNumber(safelyGetNestedProperty(system, 'alerts', 'pH', 'phMin'));
              const phMax = formatNumber(safelyGetNestedProperty(system, 'alerts', 'pH', 'phMax'));
              const phAlert = safelyGetNestedProperty(system, 'alerts', 'pH', 'phAlert');

              const tdsMin = formatNumber(safelyGetNestedProperty(system, 'alerts', 'TDS', 'tdsMin'));
              const tdsAlert = safelyGetNestedProperty(system, 'alerts', 'TDS', 'tdsAlert');

              const waterTempMin = formatNumber(safelyGetNestedProperty(system, 'alerts', 'WaterTemperature', 'waterTempMin'));
              const waterTempMax = formatNumber(safelyGetNestedProperty(system, 'alerts', 'WaterTemperature', 'waterTempMax'));
              const waterTempAlert = safelyGetNestedProperty(system, 'alerts', 'WaterTemperature', 'waterTempAlert');

              const humidityOffset = formatNumber(safelyGetNestedProperty(system, 'alerts', 'Humidity', 'humidityOffset'));
              const humidityAlert = safelyGetNestedProperty(system, 'alerts', 'Humidity', 'humidityAlert');

              const airTempMin = formatNumber(safelyGetNestedProperty(system, 'alerts', 'AirTemperature', 'airTempMin'));
              const airTempMax = formatNumber(safelyGetNestedProperty(system, 'alerts', 'AirTemperature', 'airTempMax'));
              const airTempAlert = safelyGetNestedProperty(system, 'alerts', 'AirTemperature', 'airTempAlert');

              const pumpsAlert = safelyGetNestedProperty(system, 'alerts', 'DP', 'dpAlert');

              const camAlert = safelyGetNestedProperty(system, 'alerts', 'CAM', 'camAlert');


              return (
                <tr key={system.systemName}>
                  <td>
                    <div className="system-name-container">
                      {system.systemName}
                      <img src={settingsIcon} alt="Settings" className="settings-icon" onClick={() => openModal(system.systemName)} />
                    </div>
                  </td>
                  <td>
                    <div className='min-max-alerts'>
                      <div className='thresholds-alerts'>
                        <span className='min-max-style'>Min:</span> {phMin}
                        <span className='min-max-style'>Max:</span> {phMax}
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => togglePhAlert(phAlert, () => {}, system.systemName)}>
                          <img src={phAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                          <span className="auto-label alert-label" style={{ color: phAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>
                  {/*TDS*/}
                  <td>
                    <div className='min-max-alerts'>
                      <div className='thresholds-alerts'>
                        <span className='min-max-style'>Min:</span> {tdsMin ? `${tdsMin} ppm` : '-'}
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => toggleTdsAlert(tdsAlert, () => {}, system.systemName)}>
                        <img src={tdsAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                        <span className="auto-label alert-label" style={{ color: tdsAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>

                  {/*Water Temp*/}
                  <td>
                    <div className='min-max-alerts'>
                      < div className='thresholds-alerts'>
                            <span className='min-max-style'>Min:</span> {waterTempMin ? `${waterTempMin} °C` : '-'}
                            <span className='min-max-style'>Max:</span> {waterTempMax ? `${waterTempMax} °C` : '-'}
                        </div>
                        <button className="toggle-button alerts-buttons" onClick={() => toggleWaterTempAlert(waterTempAlert, () => {}, system.systemName)}>
                            <img src={waterTempAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                            <span className="auto-label alert-label" style={{ color: waterTempAlert ? '#0096ff' : 'grey' }}>Alert</span>
                        </button>
                    </div>
                  </td>

                  {/*Humidity*/}
                  <td>
                    <div className='min-max-alerts'>
                      <div className='thresholds-alerts'>
                      <span className='min-max-style'>Offset:</span> {humidityOffset ? `${humidityOffset}%` : '-'}
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => toggleHumidityAlert(humidityAlert, () => {}, system.systemName)}>
                        <img src={humidityAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                        <span className="auto-label alert-label" style={{ color: humidityAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>

                  {/*Air Temp*/}
                  <td>
                    <div className='min-max-alerts'>
                      <div className='thresholds-alerts'>
                        <span className='min-max-style'>Min:</span> {airTempMin ? `${airTempMin} °C` : '-'}
                        <span className='min-max-style'>Max:</span> {airTempMax ? `${airTempMax} °C` : '-'}
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => toggleAirTempAlert(airTempAlert, () => {}, system.systemName)}>
                        <img src={airTempAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                        <span className="auto-label alert-label" style={{ color: airTempAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>

                  {/*Dosing Pumps*/}
                  <td>
                    <div className='min-max-alerts'>
                      <div>
                        <span className='min-max-style'>Malfunction Alert:</span>
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => toggleDpAlert(pumpsAlert, () => {}, system.systemName)}>
                        <img src={pumpsAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                        <span className="auto-label alert-label" style={{ color: pumpsAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>

                  {/*CAM*/}
                  <td>
                    <div className='min-max-alerts'>
                      <div>
                        <span className='min-max-style'>Malfunction Alert:</span>
                      </div>
                      <button className="toggle-button alerts-buttons" onClick={() => toggleCamAlert(camAlert, () => {}, system.systemName)}>
                        <img src={camAlert ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                        <span className="auto-label alert-label" style={{ color: camAlert ? '#0096ff' : 'grey' }}>Alert</span>
                      </button>
                    </div>
                  </td>
                </tr>
              );
            })}
          </tbody>
        </table>
        <div className="scroll-indicator" onClick={scrollToRight}>
          <h5>Scroll for more</h5>
          <div>
            <img src={scroll} alt="Scroll" className='scroll' />
          </div>
        </div>
      </div>
      {isModalOpen && (
          <AlertsSettingsModal 
              systemName={selectedSystem} 
              onClose={() => setIsModalOpen(false)} 
              isDarkMode={isDarkMode} // Pass isDarkMode as a prop
          />
      )}
    </div>
  );
};

export default SystemAlerts;
