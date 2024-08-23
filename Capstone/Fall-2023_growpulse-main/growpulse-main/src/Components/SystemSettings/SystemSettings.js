import React, {useCallback, useEffect, useState, useRef} from 'react';
import {auth, database} from "../../firebaseConfig";
import { fetchPhMinMax, updatePhMinMax, togglePhAuto } from '../Services/phServices';
import {fetchLightTimes, updateLightStart, updateLightEnd, toggleLightScheduleAuto} from '../Services/LightServices';
import {fetchHumidityData, updateHumidityLevel, toggleAutomationHumidity} from '../Services/HumidityServices';
import {fetchInitialDosingValues, updateInitialDosingValues, toggleStartInitialDosing} from '../Services/InitialDosingServices'
import './SystemSettings.css';
import {createPhChart, createTdsChart, fetchLastSevenSamples} from "../Services/chartsServices";
import { ref, onValue, off } from 'firebase/database';
import {fetchPhAutoStatus, fetchPhValue} from "../Services/phServices";
import {fetchTdsValue} from "../Services/tdsServices";
import * as pumpService from "../Services/DosingPumpsServices";
import on from '../Images/Dashboard/ON.png';
import off_icon from '../Images/Dashboard/OFF.png';
import off_white from '../Images/Dashboard/ON-white.png';
import {useParams} from "react-router-dom";

const SystemSettings = ({ sidebarExpanded, isDarkMode }) => {
    const [phBalanceAuto, setPhBalanceAuto] = useState(false);
    const [lightScheduleAuto, setLightScheduleAuto] = useState(false);
    const [humidityControlAuto, setHumidityControlAuto] = useState(false);
    const [startInitialDosing, setStartInitialDosing] = useState(false);
    const [amountSolutionA, setAmountSolutionA] = useState(0.5);
    const [amountSolutionB, setAmountSolutionB] = useState(0.5);
    const [systemVolume, setSystemVolume] = useState(2.5);
    const [dispensePumpA, setDispensePumpA] = useState(false);
    const [dispensePumpB, setDispensePumpB] = useState(false);
    const [dispensePumpC, setDispensePumpC] = useState(false);
    const [dispensePumpD, setDispensePumpD] = useState(false);
    const [amountPumpA, setAmountPumpA] = useState(0.0);
    const [amountPumpB, setAmountPumpB] = useState(0.0);
    const [amountPumpC, setAmountPumpC] = useState(0.0);
    const [amountPumpD, setAmountPumpD] = useState(0.0);
    const [phMin, setPhMin] = useState(5.5);
    const [phMax, setPhMax] = useState(6.5);
    const [lightStart, setLightStart] = useState(null);
    const [lightEnd, setLightEnd] = useState(null);
    const [humidity, setHumidity] = useState(65);
    const { systemName } = useParams();
    const [hasUnsavedChanges, setHasUnsavedChanges] = useState(false);
    const [hasUnsavedChangesInitialDosing, sethasUnsavedChangesInitialDosing] = useState(false);

    const [flashUpdate, setFlashUpdate] = useState(false);
    const [recentSamples, setRecentSamples] = useState({
        TDS: [],
        pH: [],
        Times: []
    });

    const [phValue, setPhValue] = useState(null);
    const [phAuto, setPhAuto] = useState(false);
    const [dp1Status, setDP1Status] = useState(false);
    const [dp2Status, setDP2Status] = useState(false); 
    const phChartRef = useRef(null);

    const [tdsValue, setTdsValue] = useState(null);
    const [dp3Status, setDP3Status] = useState(false);
    const [dp4Status, setDP4Status] = useState(false); 
    const TdsChartRef = useRef(null);

    const [initialValues, setInitialValues] = useState({
        phMin: null,
        phMax: null,
        lightStart: null,
        lightEnd: null,
        humidity: null,
        phBalanceAuto: null,
        lightScheduleAuto: null,
        humidityControlAuto: null,
        amountSolutionA: null,
        amountSolutionB:null,
        systemVolume: null,
        startInitialDosing:null,
    });

    const handleLightStartTimeChange = (event) => {
        const newStartTime = event.target.value;
        setLightStart(newStartTime);
    };

    const handleLightEndTimeChange = (event) => {
        const newEndTime = event.target.value;
        setLightEnd(newEndTime);
    };

    const handleChange = (currentValue, operation, setValue, limitMin = 0, limitMax = 14) => {
        let newValue = currentValue;

        if (operation === 'increment') {
            newValue = parseFloat(currentValue) + 0.1;
            if (newValue > limitMax) {
                newValue = limitMax;
            }
        } else if (operation === 'decrement') {
            newValue = parseFloat(currentValue) - 0.1;
            if (newValue < limitMin) {
                newValue = limitMin;
            }
        }
        setValue(newValue.toFixed(1)); // Round to 1 decimal place
    }

    useEffect(() => {
        // Reset to default values when systemName changes
        setPhBalanceAuto(false);
        setLightScheduleAuto(false);
        setHumidityControlAuto(false);
        setStartInitialDosing(false);
        setAmountSolutionA(0.5);
        setAmountSolutionB(0.5);
        setSystemVolume(2.5);
        setDispensePumpA(false);
        setDispensePumpB(false);
        setDispensePumpC(false);
        setDispensePumpD(false);
        setAmountPumpA(0.0);
        setAmountPumpB(0.0);
        setAmountPumpC(0.0);
        setAmountPumpD(0.0);
        setPhMin(5.5);
        setPhMax(6.5);
        setLightStart(0);
        setLightEnd(0);
        setHumidity(65);

        fetchPhMinMax(setPhMin, setPhMax, systemName);
        fetchLightTimes(setLightStart, setLightEnd, setLightScheduleAuto, systemName);
        fetchHumidityData(setHumidity, setHumidityControlAuto, systemName);
        fetchInitialDosingValues(setStartInitialDosing, setAmountSolutionA, setAmountSolutionB, setSystemVolume, systemName);

        const currentUser = auth.currentUser;

        if (currentUser) {
            const phAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phAuto`);
            const lightAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/LightAuto`);
            const humidityAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/automationHumidity`);
            const startInitialDosingRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/startInitialDosing`);

            // Subscribe to changes
            const unsubscribePhAuto = onValue(phAutoRef, (snapshot) => {
                const phAutoValue = snapshot.val();
                setPhBalanceAuto(phAutoValue ?? false);
            });

            const unsubscribeLightAuto = onValue(lightAutoRef, (snapshot) => {
                const lightAutoValue = snapshot.val();
                setLightScheduleAuto(lightAutoValue ?? false);
            });

            const unsubscribeHumidityAuto = onValue(humidityAutoRef, (snapshot) => {
                const humidityAutoValue = snapshot.val();
                setHumidityControlAuto(humidityAutoValue ?? false);
            });

            const unsubscribeStartInitialDosing = onValue(startInitialDosingRef, (snapshot) => {
                const startInitialDosingValue = snapshot.val();
                setStartInitialDosing(startInitialDosingValue ?? false);
            });

            // Cleanup function to unsubscribe from the listeners
            return () => {
                off(phAutoRef, 'value', unsubscribePhAuto);
                off(lightAutoRef, 'value', unsubscribeLightAuto);
                off(humidityAutoRef, 'value', unsubscribeHumidityAuto);
                off(startInitialDosingRef, 'value', unsubscribeStartInitialDosing);
            };
        }
    }, [systemName]);

    useEffect(() => {
        setInitialValues({
            phMin,
            phMax,
            lightStart,
            lightEnd,
            humidity,
            amountSolutionA,
            amountSolutionB,
            systemVolume,
        });
    // eslint-disable-next-line react-hooks/exhaustive-deps
    }, []);

    const saveChanges = () => {
            updatePhMinMax(phMin, phMax, systemName);
            updateLightStart(lightStart, systemName);
            updateLightEnd(lightEnd, systemName);
            updateHumidityLevel(humidity, systemName);
            setHasUnsavedChanges(false);
        }, saveDosingSettings = () => {
            updateInitialDosingValues(amountSolutionA, amountSolutionB, systemVolume, systemName);
            sethasUnsavedChangesInitialDosing(false);
        }, checkForUnsavedChanges = useCallback(() => {
            if (
                initialValues.phMin !== phMin ||
                initialValues.phMax !== phMax ||
                initialValues.lightStart !== lightStart ||
                initialValues.lightEnd !== lightEnd ||
                initialValues.humidity !== humidity
            ) {
                setHasUnsavedChanges(true);
            } else {
                setHasUnsavedChanges(false);
            }
        }, [initialValues, phMin, phMax, lightStart, lightEnd, humidity]),
        checkForUnsavedInitialDosing = useCallback(() => {
            if (
                initialValues.amountSolutionA !== amountSolutionA ||
                initialValues.amountSolutionB !== amountSolutionB ||
                initialValues.systemVolume !== systemVolume
            ) {
                sethasUnsavedChangesInitialDosing(true);
            } else {
                sethasUnsavedChangesInitialDosing(false);
            }
        }, [initialValues, amountSolutionA, amountSolutionB, systemVolume]);

    useEffect(() => {
        checkForUnsavedChanges();
    }, [phMin, phMax, lightStart, lightEnd, humidity, checkForUnsavedChanges]);

    useEffect(() => {
        checkForUnsavedInitialDosing();
    }, [amountSolutionA, amountSolutionB, systemVolume, checkForUnsavedInitialDosing]);

    /* pH */
    /* Live Feed Fetching */
    useEffect(() => {
        setPhValue(0);
        setPhAuto(false);

        fetchPhValue((newPhValue) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            setPhValue(newPhValue);
        }, systemName);

        fetchPhAutoStatus(setPhAuto, systemName);
    }, [systemName]);

    /* TDS */
    /* Live Feed Fetching */
    useEffect(() => {
        setTdsValue(0);

        fetchTdsValue((newTdsValue) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            setTdsValue(newTdsValue);
        }, systemName);

    }, [systemName]);

    /* Button State Fetching */ 
    // For DP1
    useEffect(() => {
        const unsubscribeDP1 = pumpService.fetchDP1Status(setDP1Status, systemName);
        return unsubscribeDP1;
    }, [systemName]);

    useEffect(() => {
        const unsubscribeDP2 = pumpService.fetchDP2Status(setDP2Status, systemName);
        return unsubscribeDP2;
    }, [systemName]);

    useEffect(() => {
        const unsubscribeDP3 = pumpService.fetchDP3Status(setDP3Status, systemName);
        return unsubscribeDP3;
    }, [systemName]);

    useEffect(() => {
        const unsubscribeDP4 = pumpService.fetchDP4Status(setDP4Status, systemName);
        return unsubscribeDP4;
    }, [systemName]);

    /* Live Feed Charts */
    const initializeCharts = useCallback(() => {

        setTimeout(() => {
            const phCtx = document.getElementById('phChart');
            const tdsCtx = document.getElementById('tdsChart');

            if (recentSamples.pH.length > 0) {
                phChartRef.current = createPhChart(phCtx.getContext('2d'), recentSamples, phChartRef, isDarkMode);
            } 
            if (recentSamples.TDS.length > 0) {
                TdsChartRef.current = createTdsChart(tdsCtx.getContext('2d'), recentSamples, TdsChartRef, isDarkMode);
            }

        }, 0);
    }, [recentSamples, isDarkMode]);

    useEffect(() => {
        initializeCharts();
    }, [recentSamples, initializeCharts]);

    const resetChartData = () => {
        // Destroy existing charts
        if (phChartRef.current) phChartRef.current.destroy();
        if (TdsChartRef.current) TdsChartRef.current.destroy();
    
        // Reset chart data
        setRecentSamples({
            TDS: [],
            pH: [],
            Times: []
        });
    };

    useEffect(() => {

        resetChartData();
        fetchLastSevenSamples(setRecentSamples, systemName);

    }, [systemName]);

    return (
        <div className={`background-overlay ${isDarkMode ? 'dark-mode' : ''} ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'}`}> {/* css file in src/Components/Common */}
            <div className={`systemControl ${isDarkMode ? 'dark-mode' : ''}`}>
                <h2>{systemName}'s Settings:</h2>
                <div className='system-Control-Live-Feed'>
                    {/*pH*/}
                    <div className="container ph-LiveFeed">
                        <h3>pH</h3>
                        <div className="control">
                            <div>
                                <p className={flashUpdate ? 'flash-animation' : ''}>{phValue}</p>
                            </div>
                            <div className="control auto">
                                <div className="control buttons">
                                    <button
                                        className={`arrow-button ${dp1Status ? 'active' : ''}`}
                                        onClick={() => pumpService.handleTogglePump('DP1', systemName)}
                                    >
                                        &#9650;
                                    </button>
                                    <button
                                        className={`arrow-button ${dp2Status ? 'active' : ''}`}
                                        onClick={() => pumpService.handleTogglePump('DP2', systemName)}
                                    >
                                        &#9660;
                                    </button>
                                </div>
                                <h5 style={phAuto ? { color: '#52e000', fontSize: '16px' } : {}} >Auto</h5>
                            </div>
                        </div>
                        <div className="chart">
                            <canvas id="phChart"></canvas>
                        </div>
                    </div>

                    {/*TDS*/}
                    <div className="container TDS-LiveFeed">
                        <h3>TDS</h3>
                        <div className="control">
                            <div>
                                <p className={flashUpdate ? 'flash-animation' : ''}>{tdsValue} ppm</p>
                            </div>
                            <div className="control auto tds-buttons">
                                <div className="control button">
                                    <button
                                        className={`arrow-button ${dp3Status ? 'active' : ''}`}
                                        onClick={() => pumpService.handleTogglePump('DP3',systemName)}
                                    >
                                        A
                                    </button>
                                    <button
                                        className={`arrow-button ${dp4Status ? 'active' : ''}`}
                                        onClick={() => pumpService.handleTogglePump('DP4',systemName)}
                                    >
                                        B
                                    </button>
                                </div>
                            </div>
                        </div>
                        <div className="chart">
                            <canvas id="tdsChart"></canvas>
                        </div>
                    </div>
                </div>
                <div className="table-container">
                    <table className="system-table">
                        <thead>
                        <tr>
                            <th>Type</th>
                            <th colSpan="2">Thresholds</th>
                            <th>Status</th>
                            <th>Action</th>
                        </tr>
                        </thead>
                        <tbody>
                        {/* Automation Section */}
                        <tr>
                            <td colSpan="4" className="dropdown">Automation</td>
                            <td className="save-cell">
                                {hasUnsavedChanges && (
                                    <button className="save-button" onClick={saveChanges}>
                                        Save
                                    </button>
                                )}
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>pH Balance</td>
                            <td>
                                <span>Min:</span>
                                <div className="input-container">
                                    <button onClick={() => handleChange(phMin, 'decrement', setPhMin, 0, phMax)}> - </button>
                                    <input
                                        type="text"
                                        value={phMin}
                                        onChange={(e) => updatePhMinMax("phMin", e.target.value, setPhMin, setPhMax, systemName)}
                                        readOnly
                                    />
                                    <button onClick={() => handleChange(phMin, 'increment', setPhMin, 0, phMax)}> + </button>
                                </div>
                            </td>
                            <td>
                                <span>Max:</span>
                                <div className="input-container">
                                    <button onClick={() => handleChange(phMax, 'decrement', setPhMax, phMin, 14)}> - </button>
                                    <input
                                        type="text"
                                        value={phMax}
                                        onChange={(e) => updatePhMinMax("phMax", e.target.value, setPhMin, setPhMax, systemName)}
                                        readOnly
                                    />
                                    <button onClick={() => handleChange(phMax, 'increment', setPhMax, phMin, 14)}> + </button>
                                </div>
                            </td>
                            <td>
                                <span>Auto Balance</span>
                                <div className="ON-OFF">
                                    <span
                                        className={phBalanceAuto ? "active ON" : "inactive ON"}
                                        style={phBalanceAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        ON
                                    </span>
                                    <span
                                        className={!phBalanceAuto ? "active OFF" : "inactive OFF"}
                                        style={!phBalanceAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        OFF
                                    </span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button
                                    className="toggle-button"
                                    onClick={() => togglePhAuto(phBalanceAuto, setPhBalanceAuto, systemName)}
                                >
                                    <img src={phBalanceAuto ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: phBalanceAuto ? '#0096ff' : "grey" }}>A</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>Light Schedule</td>
                            <td>
                                <span>Start:</span>
                                <div className="time-container">
                                    <input value={lightStart} type="time" onChange={handleLightStartTimeChange} />
                                </div>
                            </td>
                            <td>
                                <span>End:</span>
                                <div className="time-container">
                                    <input value={lightEnd} type="time" onChange={handleLightEndTimeChange} />
                                </div>
                            </td>
                            <td>
                                <span>Auto Schedule</span>
                                <div className="ON-OFF">
                                    <span
                                        className={lightScheduleAuto ? "active ON" : "inactive ON"}
                                        style={lightScheduleAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        ON
                                    </span>
                                    <span
                                        className={!lightScheduleAuto ? "active OFF" : "inactive OFF"}
                                        style={!lightScheduleAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        OFF
                                    </span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button
                                    className="toggle-button"
                                    onClick={() => toggleLightScheduleAuto(lightScheduleAuto, setLightScheduleAuto, systemName)}
                                >
                                    <img src={lightScheduleAuto ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: lightScheduleAuto ? '#0096ff' : 'grey' }}>A</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>Humidity Control</td>
                            <td colSpan="2" className="thresholds">
                                <div className="slider-container" data-min="30" data-max="100">
                                    <input
                                        type="range"
                                        min="30"
                                        max="100"
                                        value={humidity}
                                        onChange={(e) => setHumidity(e.target.value)}
                                    />
                                    <span
                                        className="humidity-value"
                                        style={{ left: `calc(15% + (70% * (${humidity} - 30) / 70) - 20px)` }}>
                                        {humidity}%
                                    </span>
                                </div>
                            </td>
                            <td>
                                <span>Auto Control</span>
                                <div className="ON-OFF">
                                    <span
                                        className={humidityControlAuto ? "active ON" : "inactive ON"}
                                        style={humidityControlAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        ON
                                    </span>
                                    <span
                                        className={!humidityControlAuto ? "active OFF" : "inactive OFF"}
                                        style={!humidityControlAuto ? {fontSize: 'larger'} : {}}
                                    >
                                        OFF
                                    </span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button" onClick={() => toggleAutomationHumidity(humidityControlAuto, setHumidityControlAuto, systemName)}>
                                    <img src={humidityControlAuto ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: humidityControlAuto ? '#0096ff' : 'grey' }}>A</span>
                                </button>
                            </td>
                        </tr>

                        {/* Initial Dosing Section */}
                        <tr>
                            <td colSpan="4" className="dropdown">Initial dosing</td>
                            <td className="save-cell">
                                {hasUnsavedChangesInitialDosing && (
                                    <button className="save-button" onClick={saveDosingSettings}>
                                        Save
                                    </button>
                                )}
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>Solution A</td>
                            <td colSpan="4">
                                <span className="amount-txt">Dose Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button onClick={() => handleChange(amountSolutionA, 'decrement', setAmountSolutionA)}> - </button>
                                        <input
                                            type="text"
                                            value={amountSolutionA}
                                            onChange={(e) => updateInitialDosingValues("amountSolutionA", e.target.value, setAmountSolutionA, setAmountSolutionB, setSystemVolume, systemName)}
                                            readOnly />
                                        <button onClick={() => handleChange(amountSolutionA, 'increment', setAmountSolutionA)}> + </button>
                                    </div>
                                    <span>ml/gal</span>
                                </div>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>Solution B</td>
                            <td colSpan="1">
                                <span className="amount-txt">Dose Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button onClick={() => handleChange(amountSolutionB, 'decrement', setAmountSolutionB)}> - </button>
                                        <input
                                            type="text"
                                            value={amountSolutionB}
                                            onChange={(e) => updateInitialDosingValues("amountSolutionB", e.target.value, setAmountSolutionA, setAmountSolutionB, setSystemVolume, systemName)}
                                            readOnly />
                                        <button onClick={() => handleChange(amountSolutionB, 'increment', setAmountSolutionB)}> + </button>
                                    </div>
                                    <span>ml/gal</span>
                                </div>
                            </td>
                            <td colSpan="2">
                                <div className="status">
                                    <span >Press "Start" to begin dosing process</span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button-Init-Dosing" onClick={() => toggleStartInitialDosing(startInitialDosing, setStartInitialDosing, systemName)}>
                                    <img src={startInitialDosing ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: startInitialDosing ? '#0096ff' : 'grey' }}>Start</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>System Volume</td>
                            <td colSpan="4">
                                <span className="amount-txt">Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button onClick={() => handleChange(systemVolume, 'decrement', setSystemVolume)}> - </button>
                                        <input
                                            type="text"
                                            value={systemVolume}
                                            onChange={(e) => updateInitialDosingValues("systemVolume", e.target.value, setAmountSolutionA, setAmountSolutionB, setSystemVolume, systemName)}
                                            readOnly />
                                        <button onClick={() => handleChange(systemVolume, 'increment', setSystemVolume)}> + </button>
                                    </div>
                                    <span>gal</span>
                                </div>
                            </td>
                        </tr>

                        {/* Manual Dosing Section */}
                        <tr>
                            <td colSpan="5" className="dropdown">Manual Dosing</td>
                        </tr>
                        <tr className="sub-menu">
                            <td>pump A</td>
                            <td colSpan="2">
                                <span className="amount-txt">Dispense Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button>-</button>
                                        <input type="text" value={amountPumpA} readOnly />
                                        <button>+</button>
                                    </div>
                                    <span>ml</span>
                                </div>
                            </td>
                            <td>
                                <div className="OFF">
                                    <span className="OFF">OFF</span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button" onClick={() => setDispensePumpA(!dispensePumpA)}>
                                    <img src={dispensePumpA ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: dispensePumpA ? '#0096ff' : 'grey' }}>Run</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>pump B</td>
                            <td colSpan="2">
                                <span className="amount-txt">Dispense Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button>-</button>
                                        <input type="text" value={amountPumpB} readOnly />
                                        <button>+</button>
                                    </div>
                                    <span>ml</span>
                                </div>
                            </td>
                            <td>
                                <div className="OFF">
                                    <span className="OFF">OFF</span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button" onClick={() => setDispensePumpB(!dispensePumpB)}>
                                    <img src={dispensePumpB ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: dispensePumpB ? '#0096ff' : 'grey' }}>Run</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>pump C</td>
                            <td colSpan="2">
                                <span className="amount-txt">Dispense Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button>-</button>
                                        <input type="text" value={amountPumpC} readOnly />
                                        <button>+</button>
                                    </div>
                                    <span>ml</span>
                                </div>
                            </td>
                            <td>
                                <div className="OFF">
                                    <span className="OFF">OFF</span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button" onClick={() => setDispensePumpC(!dispensePumpC)}>
                                    <img src={dispensePumpC ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: dispensePumpC ? '#0096ff' : 'grey' }}>Run</span>
                                </button>
                            </td>
                        </tr>
                        <tr className="sub-menu">
                            <td>pump D</td>
                            <td colSpan="2">
                                <span className="amount-txt">Dispense Amount:</span>
                                <div className="initial-dosing">
                                    <div className="input-container">
                                        <button>-</button>
                                        <input type="text" value={amountPumpD} readOnly />
                                        <button>+</button>
                                    </div>
                                    <span>ml</span>
                                </div>
                            </td>
                            <td>
                                <div className="OFF">
                                    <span className="OFF">OFF</span>
                                </div>
                            </td>
                            <td className="toggle-cell">
                                <button className="toggle-button" onClick={() => setDispensePumpD(!dispensePumpD)}>
                                    <img src={dispensePumpD ? on : (isDarkMode ? off_white : off_icon)} alt="Toggle" />
                                    <span className="auto-label" style={{ color: dispensePumpD ? '#0096ff' : 'grey' }}>Run</span>
                                </button>
                            </td>
                        </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    );
};

export default SystemSettings;
