import React, { useEffect, useState, useCallback } from 'react';
import { ComposableMap, Geographies, Geography, Marker } from "react-simple-maps";
import './AllSystems.css';
import '../Common/background.css';
import worldMap from './countries-110m.json';
import countriesCoordinates from './countriesCoordinates.json';
import { database, auth, firestore } from '../../firebaseConfig';
import { ref, get, update, onValue } from 'firebase/database';
import { collection, doc, getDoc, onSnapshot, addDoc, deleteDoc, updateDoc } from 'firebase/firestore';
import { fetchPhValue } from '../Services/phServices';
import { fetchTdsValue } from '../Services/tdsServices';
import { fetchAirTemperature } from '../Services/AirTempServices';
import { fetchWaterTempValue } from '../Services/WaterTempServices';
import { fetchHumidity } from '../Services/HumidityServices';
import { fetchLigthPowerStatus } from '../Services/LightServices';
import { fetchMainPumpStatus } from '../Services/MainPumpServices';
import trashIcon from '../Images/SidebarIcons/trash.png';


const AllSystems = ({ sidebarExpanded, isDarkMode}) => {
    const [systemsData, setSystemsData] = useState([]);
    const [liveFeedData, setLiveFeedData] = useState({});
    const [dispensedData, setDispensedData] = useState({});
    const [flashingSensors, setFlashingSensors] = useState({});
    const [tasks, setTasks] = useState([]);
    

    const fetchSystemLiveFeed = useCallback((systemName) => {
        const updateSensorData = (sensorName, value, isNumeric = false) => {
            let formattedValue = '-';
            if (isNumeric) {
                if (value !== null && !isNaN(parseFloat(value))) {
                    formattedValue = parseFloat(value).toFixed(2);
                }
            } else {
                formattedValue = value !== null ? value : '-';
            }
        
            setLiveFeedData(prevState => {
                // Check if the value has changed
                const hasChanged = prevState[systemName] && prevState[systemName][sensorName] !== formattedValue;
                if (hasChanged) {
                    // Set the flashing state for this sensor
                    setFlashingSensors(prevFlashing => ({
                        ...prevFlashing,
                        [`${systemName}-${sensorName}`]: true
                    }));
        
                    // Remove the flashing state after 1 second
                    setTimeout(() => {
                        setFlashingSensors(prevFlashing => ({
                            ...prevFlashing,
                            [`${systemName}-${sensorName}`]: false
                        }));
                    }, 1000);
                }
                return {
                    ...prevState,
                    [systemName]: {
                        ...prevState[systemName],
                        [sensorName]: formattedValue
                    }
                };
            });
        };
        
    
        // Initialize light and pump status to OFF if not already set
        setLiveFeedData(prevState => {
            if (!prevState[systemName]) {
                return {
                    ...prevState,
                    [systemName]: {
                        lightPower: 'OFF',
                        pumpPower: 'OFF'
                    }
                };
            }
            return prevState;
        });

        fetchPhValue(value => updateSensorData('phValue', value, true), systemName);
        fetchTdsValue(value => updateSensorData('tdsValue', value, true), systemName);
        fetchAirTemperature(value => updateSensorData('airTemperature', value, true), systemName);
        fetchWaterTempValue(value => updateSensorData('waterTemperature', value, true), systemName);
        fetchHumidity(value => updateSensorData('humidity', value), systemName);

        fetchLigthPowerStatus(value => {
            if (value === true) {
                updateSensorData('lightPower', 'ON');
            }
        }, systemName);
        fetchMainPumpStatus(value => {
            if (value === true) {
                updateSensorData('pumpPower', 'ON');
            }
        }, systemName);

    }, []);

    useEffect(() => {
        systemsData.forEach(system => {
            fetchSystemLiveFeed(system.systemName);
        });
    }, [systemsData, fetchSystemLiveFeed]);

    useEffect(() => {
        // Assuming the current user is authenticated
        const tasksColRef = collection(firestore, `Registered Users/${auth.currentUser.uid}/Tasks`);
        const unsubscribe = onSnapshot(tasksColRef, (snapshot) => {
            const fetchedTasks = [];
            snapshot.forEach(doc => fetchedTasks.push({ id: doc.id, ...doc.data() }));
            // Sort tasks by due date, from most recent to latest
            fetchedTasks.sort((a, b) => {
                const dateA = new Date(a.dueDate);
                const dateB = new Date(b.dueDate);
                return dateA - dateB; // Sorts ascending; swap `dateA` and `dateB` to sort descending
            });
            setTasks(fetchedTasks);
        });
    
        return () => unsubscribe(); // Detach listener when the component unmounts
    }, []);    
    
    const addTask = (newTask) => {
        const tasksColRef = collection(firestore, `Registered Users/${auth.currentUser.uid}/Tasks`);
        addDoc(tasksColRef, newTask);
    };

    const updateTaskName = (taskId, newName) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        updateDoc(taskDocRef, { name: newName });
    };
    
    const updateTaskDueDate = (taskId, newDueDate) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        updateDoc(taskDocRef, { dueDate: newDueDate });
    };
    
    const deleteTask = (taskId) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        deleteDoc(taskDocRef);
    };
    
    const toggleTaskCompleted = (taskId, completed) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        updateDoc(taskDocRef, { completed: !completed });
    };        

    const updateTaskPriority = (taskId, newPriority) => {
        const taskDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/Tasks/${taskId}`);
        updateDoc(taskDocRef, { priority: newPriority });
    };
    
    
    /*Map Locations*/
    const systemsCountPerCountry = systemsData.reduce((acc, system) => {
        acc[system.Location] = (acc[system.Location] || 0) + 1;
        return acc;
    }, {});

    const handleLocationChange = async (event, systemName, index) => {
        const newLocation = event.target.value;
        // Update the database
        const systemRef = ref(database, `Registered Users/${auth.currentUser.uid}/${systemName}`);
    
        try {
          await update(systemRef, { Location: newLocation });
          // Update the state
          const updatedSystemsData = [...systemsData];
          updatedSystemsData[index].Location = newLocation;
          setSystemsData(updatedSystemsData);
        } catch (error) {
          console.error("Error updating location:", error);
        }
    };         

    /*Fetch All User's Systems*/
    useEffect(() => {
        const fetchSystemsData = async () => {
            const currentUser = auth.currentUser;
            if (currentUser) {
                const userRef = ref(database, `Registered Users/${currentUser.uid}`); 

                try {
                    const snapshot = await get(userRef); 
                    if (snapshot.exists()) {
                        const fetchedSystemsData = [];
                        snapshot.forEach(childSnapshot => {
                            const systemName = childSnapshot.key;
                            if (systemName.startsWith("System")) {
                                const { Location, Status } = childSnapshot.val();
                                fetchedSystemsData.push({ systemName, Location, Status });
                            }
                        });
                        setSystemsData(fetchedSystemsData);
                    } else {
                        console.log("No systems available");
                    }
                } catch (error) {
                    console.error("Error fetching systems:", error);
                }
            }
        };

        fetchSystemsData();
    }, []);

    useEffect(() => {
        const currentUser = auth.currentUser;
        if (currentUser) {
            const userRef = ref(database, `Registered Users/${currentUser.uid}`);
    
            const unsubscribe = onValue(userRef, (snapshot) => {
                const fetchedSystemsData = [];
                snapshot.forEach(childSnapshot => {
                    const systemName = childSnapshot.key;
                    if (systemName.startsWith("System")) {
                        const { Location, Status } = childSnapshot.val();
                        fetchedSystemsData.push({ systemName, Location, Status });
                    }
                });
                setSystemsData(fetchedSystemsData);
            }, {
                onlyOnce: false
            });
    
            return () => unsubscribe();
        }
    }, []);

    /*Dispense Amounts*/
    useEffect(() => {
        const fetchDispensedTotals = async (systemName) => {
            // Correct path for Firestore document
            const dispensedDocRef = doc(firestore, `Registered Users/${auth.currentUser.uid}/${systemName}/DispenseHistory`);
            try {
                const docSnapshot = await getDoc(dispensedDocRef);
                if (docSnapshot.exists()) {
                    const dispensedTotals = docSnapshot.data();
                    setDispensedData(prevState => ({
                        ...prevState,
                        [systemName]: {
                            A: dispensedTotals.totalA !== null && dispensedTotals.totalA !== undefined ? dispensedTotals.totalA + ' mL' : '-',
                            B: dispensedTotals.totalB !== null && dispensedTotals.totalB !== undefined ? dispensedTotals.totalB + ' mL' : '-',
                            Up: dispensedTotals.totalUp !== null && dispensedTotals.totalUp !== undefined ? dispensedTotals.totalUp + ' mL' : '-',
                            Down: dispensedTotals.totalDown !== null && dispensedTotals.totalDown !== undefined ? dispensedTotals.totalDown + ' mL' : '-',
                        }
                    }));
                }
            } catch (error) {
                console.error("Error fetching dispensed totals:", error);
            }
        };
    
        systemsData.forEach(system => {
            fetchDispensedTotals(system.systemName);
        });
    }, [systemsData]);    

    const calculateTotalDispensed = () => {
        let totalA = 0, totalB = 0, totalUp = 0, totalDown = 0;
        let countA = 0, countB = 0, countUp = 0, countDown = 0;
    
        Object.values(dispensedData).forEach(system => {
            if(system.A !== '-') {
                totalA += parseFloat(system.A) || 0;
                countA++;
            }
            if(system.B !== '-') {
                totalB += parseFloat(system.B) || 0;
                countB++;
            }
            if(system.Up !== '-') {
                totalUp += parseFloat(system.Up) || 0;
                countUp++;
            }
            if(system.Down !== '-') {
                totalDown += parseFloat(system.Down) || 0;
                countDown++;
            }
        });
    
        return { 
            totalA: countA > 0 ? totalA + ' mL' : '-',
            totalB: countB > 0 ? totalB + ' mL' : '-',
            totalUp: countUp > 0 ? totalUp + ' mL' : '-',
            totalDown: countDown > 0 ? totalDown + ' mL' : '-'
        };
    };     

    return (
        <div className={`background-overlay ${isDarkMode ? 'dark-mode' : ''} ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'}`}>
            <div className={`allSystems ${isDarkMode ? 'dark-mode' : ''}`}>
                <header>
                    <h2 className="Header">Quick view of all of your systems:</h2>
                </header>
                <main className='overview-main'>
                    <div className="map-container">
                        <ComposableMap>
                            <Geographies geography={worldMap}>
                                {({ geographies }) =>
                                    geographies.map(geo => (
                                        <Geography
                                            key={geo.rsmKey}
                                            geography={geo}
                                            className="geography"
                                            onClick={(event) => event.preventDefault()}
                                            style={{
                                                default: { fill: "#00000000", stroke: "#888888" },
                                                hover: { fill: "#006effb7" },
                                                pressed: { fill: "#006effb7" }
                                            }}
                                        />
                                    ))
                                }
                            </Geographies>
                            {Object.entries(systemsCountPerCountry).map(([country, count]) => {
                                const { coordinates } = countriesCoordinates[country] || {};
                                if (!coordinates) return null;
                                return (
                                <Marker key={country} coordinates={coordinates}>
                                    <circle r={17} fill="#0099ff" stroke="#fff" strokeWidth={2} />
                                    <text textAnchor="middle" y={5} style={{ fontSize: 24, fontWeight: 'bold', fill: '#FFF' }}>
                                    {count}
                                    </text>
                                </Marker>
                                );
                            })}
                        </ComposableMap>
                    </div>
                    <div className="container current-systems">
                        <h3>Your Current Systems</h3>
                        <div className="table-curr-sys">
                            <table className="table-header">
                                <thead>
                                    <tr>
                                        <th>System</th>
                                        <th>Location</th>
                                        <th>Status</th>
                                    </tr>
                                </thead>
                            </table>
                            <table className="table-body">
                                <tbody>
                                    {systemsData.map((system, index) => (
                                    <tr key={system.systemName}>
                                        <td>{system.systemName}</td>
                                        <td>
                                        <select
                                            value={system.Location}
                                            className="custom-select location"
                                            onChange={(e) => handleLocationChange(e, system.systemName, index)}>
                                            {Object.keys(countriesCoordinates).map(country => (
                                            <option key={country} value={country}>{country}</option>
                                            ))}
                                        </select>
                                        </td>
                                        <td className={system.Status ? 'status-good' : 'status-bad'}>
                                        {system.Status ? "Good" : "Needs Attention!"}
                                        </td>
                                    </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    </div>
                    <div className="container Live-Feed-AllSystems">
                        <h3>Live Feed</h3>
                        <div className="table-live-feed">
                            <table>
                                <thead>
                                    <tr>
                                        <th>System</th>
                                        <th>pH</th>
                                        <th>TDS</th>
                                        <th>Water Temp.</th>
                                        <th>Humidity</th>
                                        <th>Air Temp.</th>
                                        <th>Light</th>
                                        <th>Main Pump</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {systemsData.map(system => (
                                        <tr key={system.systemName}>
                                            <td> {system.systemName} </td>
                                            <td className={flashingSensors[`${system.systemName}-phValue`] ? 'flash' : ''}>
                                                {liveFeedData[system.systemName]?.phValue ?? '-'}
                                            </td>
                                            <td className={flashingSensors[`${system.systemName}-tdsValue`] ? 'flash' : ''}>
                                                {liveFeedData[system.systemName]?.tdsValue ? `${liveFeedData[system.systemName].tdsValue} ppm` : '-'}
                                            </td>
                                            <td className={flashingSensors[`${system.systemName}-waterTemperature`] ? 'flash' : ''}>
                                                {liveFeedData[system.systemName]?.waterTemperature ? `${liveFeedData[system.systemName].waterTemperature} °C` : '-'}
                                            </td>
                                            <td className={flashingSensors[`${system.systemName}-humidity`] ? 'flash' : ''}>
                                                {liveFeedData[system.systemName]?.humidity ? `${liveFeedData[system.systemName].humidity} %` : '-'}
                                            </td>
                                            <td className={flashingSensors[`${system.systemName}-airTemperature`] ? 'flash' : ''}>
                                                {liveFeedData[system.systemName]?.airTemperature ? `${liveFeedData[system.systemName].airTemperature} °C` : '-'}
                                            </td>
                                            <td className={liveFeedData[system.systemName]?.lightPower === 'OFF' ? 'status-bad' : 'status-good'}>
                                                {liveFeedData[system.systemName]?.lightPower ?? '-'}
                                            </td>
                                            <td className={liveFeedData[system.systemName]?.pumpPower === 'OFF' ? 'status-bad' : 'status-good'}>
                                                {liveFeedData[system.systemName]?.pumpPower ?? '-'}
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    </div>
                    <div className="container indiv-costs">
                        <h3>Amount Dispensed per System</h3>
                        <div className="amount-disp">
                            <table>
                                <thead>
                                    <tr>
                                        <th>System</th>
                                        <th>A</th>
                                        <th>B</th>
                                        <th>Up</th>
                                        <th>Down</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {systemsData.map(system => (
                                        <tr key={system.systemName}>
                                            <td>{system.systemName}</td>
                                            <td>{dispensedData[system.systemName]?.A || '-'}</td>
                                            <td>{dispensedData[system.systemName]?.B || '-'}</td>
                                            <td>{dispensedData[system.systemName]?.Up || '-'}</td>
                                            <td>{dispensedData[system.systemName]?.Down || '-'}</td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    </div>
                    <div className="container total-costs">
                        <h3>Total Amount Dispensed</h3>
                        <h5>Total amount of solutions used: </h5>
                        <div className="total-dispensed-grid">
                            <div className="grid-item">
                                <span className="amount">{calculateTotalDispensed().totalA}</span>
                                <span className="label">of Sol. A</span>
                            </div>
                            <div className="grid-item">
                                <span className="amount">{calculateTotalDispensed().totalB}</span>
                                <span className="label">of Sol. B</span>
                            </div>
                            <div className="grid-item">
                                <span className="amount">{calculateTotalDispensed().totalUp}</span>
                                <span className="label">of pH Up</span>
                            </div>
                            <div className="grid-item">
                                <span className="amount">{calculateTotalDispensed().totalDown}</span>
                                <span className="label">of pH Down</span>
                            </div>
                        </div>
                    </div>
                    <div className="container task-list-container">
                        <h3>Task List</h3>
                        <button className = "add-task" onClick={() => addTask({ name: 'New Task', completed: false, dueDate: '', priority: 'Low' })}>Add New Task</button>
                        <ul className="task-list">
                            {tasks.map(task => (
                                <li key={task.id} className={`task-item ${task.completed ? 'completed' : ''}`}>
                                    <div className="task-details">
                                        <input
                                            className="checkbox"
                                            type="checkbox"
                                            checked={task.completed}
                                            onChange={() => toggleTaskCompleted(task.id, task.completed)}
                                        />
                                        <input
                                            type="text"
                                            value={task.name}
                                            onChange={(e) => updateTaskName(task.id, e.target.value)}
                                            className="task-name"
                                            disabled={task.completed}
                                        />
                                        <input
                                            type="date"
                                            value={task.dueDate || ''}
                                            onChange={(e) => updateTaskDueDate(task.id, e.target.value)}
                                            className="task-due-date"
                                            disabled={task.completed}
                                        />
                                        <select
                                            value={task.priority}
                                            onChange={(e) => updateTaskPriority(task.id, e.target.value)}
                                            className={`task-priority priority-${task.priority}`}
                                            disabled={task.completed}>
                                            <option value="Low">Low</option>
                                            <option value="Medium">Medium</option>
                                            <option value="High">High</option>
                                        </select>
                                    </div>
                                    <button onClick={() => deleteTask(task.id)} className="delete-task-button">
                                        <img src={trashIcon} alt="Delete" />
                                    </button>
                                </li>
                            ))}
                        </ul>
                    </div>
                </main>
            </div>
        </div>
    );
};

export default AllSystems;
