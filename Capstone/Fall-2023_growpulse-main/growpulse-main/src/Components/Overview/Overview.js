import React, { useEffect, useState,  useRef, useCallback, useMemo } from 'react';
import {useParams} from "react-router-dom";
import Chart from 'chart.js/auto';
import 'chartjs-adapter-moment';
import moment from 'moment';
import { fetchImage} from '../Services/CameraServices';
import {fetchPhAutoStatus, fetchPhValue} from "../Services/phServices";
import {fetchTdsValue, fetchMLPrediction} from "../Services/tdsServices";
import {fetchWaterTempValue} from "../Services/WaterTempServices";
import {fetchAirTemperature} from "../Services/AirTempServices";
import { fetchHumidity } from '../Services/HumidityServices';
import * as pumpService from "../Services/DosingPumpsServices";
import {createPhChart, createTdsChart, createWaterTemperatureChart, createAirTemperatureChart, fetchLastSevenSamples, createHumidityChart, fetchDayAverages, getChartData, fetchLiveData, getDailyChartData} from "../Services/chartsServices";
import { fetchLogHistory } from '../Services/HistoryServices';
import { handleToggleMainPump, fetchMainPumpStatus } from '../Services/MainPumpServices';
import { calculateDayNightDurations, handleToggleLight, fetchLightTimes, fetchLigthPowerStatus, createLightScheduleGanttChart } from '../Services/LightServices';

import on from "../Images/Dashboard/ON.png";
import offWhite from "../Images/Dashboard/ON-white.png";
import off from "../Images/Dashboard/OFF.png";

import './Overview.css';

const Overview = ({ sidebarExpanded, isDarkMode}) => {
    const {systemName } = useParams();
    const [imageUrl, setImageUrl] = useState('');
    const [mlPrediction, setMLPrediction] = useState(null);
    const translateAndColorDeficiency = (abbreviation) => {
        const mappings = {
            'FN': {text: 'Fully Nutritional', color: 'green'},
            'N': {text: 'Nitrogen Deficiency', color: 'red'},
            'K': {text: 'Potassium Deficiency', color: 'red'},
            'P': {text: 'Phosphorus Deficiency', color: 'red'}
        };
    
        return mappings[abbreviation] || {text: abbreviation, color: 'grey'}; 
    };
    
    
    const [flashUpdate, setFlashUpdate] = useState(false);
    const [recentSamples, setRecentSamples] = useState({
        TDS: [],
        pH: [],
        AirTemperature: [],
        WaterTemperature: [],
        Humidity: [],
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

    const [waterTempValue, setWaterTempValue] = useState(null);
    const waterTempChartRef = useRef(null);
    
    const [MainPumpOn, setMainPumpOn] = useState(false);

    const [airTempValue, setAirTempValue] = useState(null);
    const airTempChartRef = useRef(null);

    const HumidityChartRef = useRef(null);
    const [humidityValue, setHumidityValue] = useState(null);
    //const [humidityAuto] = useState(false);
    //const [humidifierOn, setHumidifierOn] = useState(false);

    const [lightPwrOn, setLightPwrOn] = useState(null);
    const [lightON, setLightON] = useState(null);
    const [lightOFF, setLightOFF] = useState(null);
    const [lightAuto, setLightAuto] = useState(false);
    const { dayDuration, nightDuration } = lightON && lightOFF ? calculateDayNightDurations(lightON, lightOFF) : { dayDuration: 0, nightDuration: 0 };
    const dayHours = Math.floor(dayDuration / 60);
    const nightHours = Math.floor(nightDuration / 60);

    const [logHistory, setLogHistory] = useState([]);

    const avgChartRef = useRef(null);
    const [dayAverages, setDayAverages] = useState([]);
    const avgChartContainerRef = useRef(null);

    const [liveData, setLiveData] = useState([]);
    const dailyChartRef = useRef(null);

    /* Camera */
    /* Image Fetching */
    const loadImage = useCallback(async () => {
        const url = await fetchImage(systemName);
        setImageUrl(url); 
    }, [systemName]);

    /* pH */
    /* Live Feed Fetching */
    useEffect(() => {
        setPhValue(0);

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

    /* Water Temperature */
    /* Live Feed Fetching */
    useEffect(() => {
        setWaterTempValue(0);

        fetchWaterTempValue((newWaterTempValue) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            setWaterTempValue(newWaterTempValue);
        }, systemName);

    }, [systemName]);

    /* Air Temperature */
    /* Live Feed Fetching */
    useEffect(() => {
        setAirTempValue(0);

        fetchAirTemperature((newAirTempValue) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            setAirTempValue(newAirTempValue);
        }, systemName);

    }, [systemName]);

    /*Humidity*/
    /* ON/OFF Button functionality (empty for now) */
    /*
        const toggleHumidity = () => {
            setHumidifierOn(currentState => !currentState);
        }; 

        useEffect(() => {
            setHumidifierOn(false);
        }, [systemName]);
    */

    /* Live Feed Fetching */
    useEffect(() => {
        setHumidityValue(0);

        fetchHumidity((newHumidityValue) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            setHumidityValue(newHumidityValue);
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


    /*Main Pump*/
    useEffect(() => {
        const unsubscribe = fetchMainPumpStatus(setMainPumpOn, systemName);
        // Clean up the subscription
        return unsubscribe;
    }, [systemName]);
    

    const toggleMainPump = async () => {
        try {
          await handleToggleMainPump(systemName, !MainPumpOn); 
          setMainPumpOn(!MainPumpOn); 
        } catch (error) {
          console.error('Error toggling the main pump:', error);
        }
    };

    /* ML prediction */
    useEffect(() => {
        setMLPrediction(null); 

        fetchMLPrediction((newMLPrediction) => {
            setFlashUpdate(true);

            setTimeout(() => {
                setFlashUpdate(false);
            }, 1000);

            const predictionInfo = translateAndColorDeficiency(newMLPrediction);
            setMLPrediction(predictionInfo); 
        }, systemName);

    }, [systemName]); 

    
    /*Light Control*/
    useEffect(() => {
        setLightON("00:00");
        setLightOFF("12:00");

        const cleanup = fetchLightTimes(setLightON, setLightOFF, setLightAuto, systemName);

        return () => {
            cleanup();
        };

    }, [systemName]);

    useEffect(() => {
        const unsubscribe = fetchLigthPowerStatus(setLightPwrOn, systemName);
        // Clean up the subscription
        return unsubscribe;
    }, [systemName]);
    
    const toggleLight = async () => {
        try {
            await handleToggleLight(systemName); 
            } catch (error) {
            console.error('Error toggling the light power:', error);
        }
    };

    useEffect(() => {
        if (lightON && lightOFF) {
            createLightScheduleGanttChart('lightScheduleGanttChartContainer', lightON, lightOFF, isDarkMode);
        }
    }, [lightON, lightOFF, isDarkMode]);

    /* Dispense History */
    /* Fetching and displaying */
    useEffect(() => {
        fetchLogHistory(setLogHistory, systemName);
    }, [systemName]);

    /* Common Fetches */
    useEffect(() => {
        loadImage();
    }, [loadImage]);

    /* Live Feed Charts */
    const initializeCharts = useCallback(() => {

        setTimeout(() => {
            const phCtx = document.getElementById('phChart');
            const tdsCtx = document.getElementById('tdsChart');
            const waterTempCtx = document.getElementById('waterTempChart');
            const airTempCtx = document.getElementById('airTempChart');
            const humidityCtx = document.getElementById('HumidityChart');

            if (recentSamples.pH.length > 0) {
                phChartRef.current = createPhChart(phCtx.getContext('2d'), recentSamples, phChartRef, isDarkMode);
            } 
            if (recentSamples.TDS.length > 0) {
                TdsChartRef.current = createTdsChart(tdsCtx.getContext('2d'), recentSamples, TdsChartRef, isDarkMode);
            }
            if (recentSamples.WaterTemperature.length > 0) {
                waterTempChartRef.current = createWaterTemperatureChart(waterTempCtx.getContext('2d'), recentSamples, waterTempChartRef, isDarkMode);
            }
            if (recentSamples.AirTemperature.length > 0) {
                airTempChartRef.current = createAirTemperatureChart(airTempCtx.getContext('2d'), recentSamples, airTempChartRef, isDarkMode);
            }
            if (recentSamples.Humidity.length > 0) {
                HumidityChartRef.current = createHumidityChart(humidityCtx.getContext('2d'), recentSamples, HumidityChartRef, isDarkMode);
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
        if (waterTempChartRef.current) waterTempChartRef.current.destroy();
        if (airTempChartRef.current) airTempChartRef.current.destroy();
        if (HumidityChartRef.current) HumidityChartRef.current.destroy();
    
        // Reset chart data
        setRecentSamples({
            TDS: [],
            pH: [],
            AirTemperature: [],
            WaterTemperature: [],
            Humidity: [],
            Times: []
        });
    };

    useEffect(() => {

        resetChartData();
        fetchLastSevenSamples(setRecentSamples, systemName);

    }, [systemName]);

    /*Daily Avg Chart*/
    useEffect(() => {
        setDayAverages([]);
        fetchDayAverages(setDayAverages, systemName);
    }, [systemName]);

    const debounce = (func, delay) => {
        let inDebounce;
        return function() {
            const context = this;
            const args = arguments;
            clearTimeout(inDebounce);
            inDebounce = setTimeout(() => func.apply(context, args), delay);
        };
    };

    useEffect(() => {
        // Function to initialize or update the chart
        const initializeOrUpdateChart = () => {
            if (window.myAvgChart) {
                window.myAvgChart.destroy();
            }

            const avgChartData = getChartData(dayAverages, isDarkMode);

            const textColor = isDarkMode ? 'white' : 'black';

            window.myAvgChart = new Chart(avgChartRef.current, {
                type: 'bar',
                data: avgChartData,
                options: {
                    animation: false,
                    scales: {
                        x: {
                            title: {
                                display: false,
                                text: 'Date',
                                color: textColor
                            },
                            ticks: {
                                color: textColor
                            }
                        },
                        y: {
                            title: {
                                display: false,
                                text: 'Sensor Values',
                            },
                            ticks: {
                                color: textColor
                            }
                        },
                        'y-axis-tds': {
                            position: 'right',
                            title: {
                                display: true,
                                text: 'TDS',
                                color: textColor
                            },
                            ticks: {
                                color: textColor
                            }
                        },
                    },
                    plugins: {
                        legend: {
                            position: 'top',
                            labels: {
                                color: textColor
                            }
                        }
                    },
                }
            });
        };

        // Initialize the chart on first render
        initializeOrUpdateChart();

        const handleResize = debounce(() => {
            // Re-initialize the chart on resize, debounced
            initializeOrUpdateChart();
        }, 100); // 100ms debounce period

        // Set up a resize observer to watch for changes in the chart container size
        const resizeObserver = new ResizeObserver(entries => {
            for (let entry of entries) {
                if (entry.target === avgChartContainerRef.current) {
                    handleResize();
                }
            }
        });

        // Start observing the chart container
        if (avgChartContainerRef.current) {
            resizeObserver.observe(avgChartContainerRef.current);
        }

        // Cleanup function to disconnect the resize observer
        return () => {
            resizeObserver.disconnect();
            if (window.myAvgChart) {
                window.myAvgChart.destroy();
            }
        };
    }, [dayAverages, isDarkMode]);

    /* Samples Data Chart */
    const getCurrentFormattedDate = () => {
        const today = new Date();
        const year = today.getFullYear();
        const month = (`0${today.getMonth() + 1}`).slice(-2); 
        const day = (`0${today.getDate()}`).slice(-2); 
        return `${year}-${month}-${day}`;
    };

    const [selectedDay, setSelectedDay] = useState(getCurrentFormattedDate());

    useEffect(() => {
        setLiveData([]);
        const fetchAndSetLiveData = async () => {
            await fetchLiveData(setLiveData, systemName);
        };

        fetchAndSetLiveData();
    }, [systemName]);

    // Effect to update Daily Chart based on selectedDay and liveData
    useEffect(() => {
        if (window.myDailyChart) {
            window.myDailyChart.destroy(); // Destroy the previous instance of the chart
        }

        const textColor = isDarkMode ? 'white' : 'black';

        if (liveData.length > 0 && dailyChartRef.current) {
            const dailyChartData = getDailyChartData(selectedDay, liveData);

            // Create the new Daily Line Chart
            window.myDailyChart = new Chart(dailyChartRef.current, {
                type: 'line',
                data: dailyChartData,
                options: {
                    animation: false,
                    scales: {
                        x: {
                            type: 'time',
                            time: {
                                unit: 'minute',
                                displayFormats: {
                                    minute: 'h:mm a'
                                },
                                tooltipFormat: 'MMM d, yyyy h:mm a',
                            },
                            title: {
                                display: true,
                                text: 'Time',
                            },
                            ticks: {
                                color: textColor,
                                source: 'data',
                                maxTicksLimit: 5,
                                maxRotation: 45,
                                minRotation: 45,
                            },
                        },
                        y: {
                            title: {
                                display: true,
                                text: 'Values',
                                color: textColor,
                            },
                            ticks: {
                                color: textColor
                            }
                        },
                        'y-axis-tds': {
                            position: 'right',
                            title: {
                                display: true,
                                text: 'TDS',
                                color: textColor,
                            },
                            min: 0,
                            max: 1500,
                            ticks: {
                                color: textColor,
                                beginAtZero: true,
                                stepSize: 100
                            },
                        },
                    },
                    plugins: {
                        legend: {
                            display: true,
                            position: 'top',
                            labels: {
                                color: textColor
                            }
                        },
                        zoom: {
                            pan: {
                                enabled: true,
                                mode: 'x',
                            },
                            zoom: {
                                wheel: {
                                    enabled: true,
                                },
                                pinch: {
                                    enabled: true,
                                },
                                mode: 'x',
                            },
                        },
                    },
                    maintainAspectRatio: false,
                    responsive: true,
                }
            });
        }
    }, [selectedDay, liveData, isDarkMode]);

    const sortedUniqueDates = useMemo(() => {
        const uniqueDates = Array.from(new Set(liveData.map((data) => data.date)));
        return uniqueDates.sort((a, b) => moment(a, 'YYYY-MM-DD').diff(moment(b, 'YYYY-MM-DD'))).reverse();
    }, [liveData]);    

    useEffect(() => {
        if (liveData.length > 0) {
            const mostRecentDate = liveData
                .map((data) => data.date)
                .sort((a, b) => moment(b, 'YYYY-MM-DD').diff(moment(a, 'YYYY-MM-DD')))[0];
            setSelectedDay(mostRecentDate);
        }
    }, [liveData]);

    return (
        <div className={`background-overlay ${sidebarExpanded ? 'sidebar-expanded' : 'sidebar-collapsed'} ${isDarkMode ? 'dark-mode' : ''}`}>
        <div className={`overview ${isDarkMode ? 'dark-mode' : ''}`}>
                <header className="header-text">
                    <h2>Here is the overview of your {systemName}:</h2>
                </header>
                <main className="overview-main">

                    {/*Camera*/}
                    <div className="container camera-container">
                        <h3>Camera</h3>
                            {imageUrl === null ? (
                                <p className="no-image">No Image Found</p>
                            ) : imageUrl ? (
                                <img className="camera" src={imageUrl} alt="Camera"/>
                            ) : (
                                <p className="no-image">Loading image...</p>
                            )}
                            {mlPrediction ? (
                                <div className="ml-prediction">
                                    <p className="prediction-text" style={{ color: mlPrediction.color }}>
                                        Deficiency Prediction: {mlPrediction.text}
                                    </p>
                                </div>
                            ) : (
                                <p className="prediction-text" style={{ color: 'grey' }}>No prediction</p>
                            )}
                    </div>

                    {/*pH*/}
                    <div className="container ph-container">
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
                    <div className="container TDS-container">
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

                    {/*Water Temperature*/}
                    <div className="container Water-Temperature-container">
                        <h3>Water Temperature</h3>
                        <div className="control">
                            <div>
                                <p className={flashUpdate ? 'flash-animation' : ''}>{waterTempValue} °C</p>
                            </div>
                        </div>
                        <div className="chart">
                            <canvas id="waterTempChart"></canvas>
                        </div>
                    </div>

                    {/*Main Pump*/}
                    <div className="container main-pump-container">
                        <h3>Main Pump</h3>
                        <div className="control">
                            <div className="control auto main-pump-button">
                                <div className="control button">
                                <button
                                    className={`on-off-button ${MainPumpOn ? 'on' : 'off'}`}
                                    onClick={toggleMainPump}
                                >
                                    <img
                                        src={MainPumpOn ? on : (isDarkMode ? offWhite : off)}
                                        alt={MainPumpOn ? "Main Pump On" : "Main Pump Off"}
                                    />
                                    <span style={{ color: MainPumpOn ? '#0096ff' : 'grey' }}>
                                        {MainPumpOn ? 'On' : 'Off'}
                                    </span>
                                </button>
                                </div>
                            </div>
                        </div>
                    </div>

                    {/*Air Temperature*/}
                    <div className="container temperature-container">
                        <h3>Air Temperature</h3>
                        <div>
                            <p className={flashUpdate ? 'flash-animation' : ''}>{airTempValue} °C</p>
                        </div>
                        <div className="chart">
                            <canvas id="airTempChart"></canvas>
                        </div>
                    </div>

                    {/*Humidity*/}
                    <div className="container humidity-container">
                        <h3>Humidity</h3>
                        <div className="control">
                            <div>
                                <p className={flashUpdate ? 'flash-animation' : ''}>{humidityValue} %</p>
                            </div>
                            <div className="control auto auto-hum">
                                {/*
                                <div className="control button">
                                    <button
                                        className="on-off-button"
                                        onClick={toggleHumidity}
                                    >
                                        <img src={humidifierOn ? on : off} alt={humidifierOn ? "Humidifier On" : "Humidifier Off"} />
                                        <span style={{ color: humidifierOn ? '#0096ff' : 'grey' }}>{humidifierOn ? 'On' : 'Off'}</span>
                                    </button>
                                </div>
                                <h5 style={humidityAuto ? { color: '#08B200', fontSize: '16px' } : {}} >Auto</h5>
                                */}
                            </div>
                        </div>
                        <div className="chart">
                            <canvas id="HumidityChart"></canvas>
                        </div>
                    </div>

                    {/*Light Control*/}
                    <div className="container light-container" key={systemName}>
                        <h3>Light Schedule</h3>
                        <div className="control">
                            <div>
                                <p>{dayHours}hrs<span className="Day-Night"> Light</span>/{nightHours}hrs<span className="Day-Night"> Dark</span></p>
                            </div>
                            <div className="control auto auto-hum">
                                <div className="control button">
                                <button
                                        className={`on-off-button ${lightPwrOn ? 'on' : 'off'}`}
                                        onClick={toggleLight}
                                        >
                                        <img
                                            src={lightPwrOn ? on : (isDarkMode ? offWhite : off)}
                                            alt={lightPwrOn ? "light Power On" : "light Power Off"}
                                        />
                                        <span style={{ color: lightPwrOn ? '#0096ff' : 'grey' }}>
                                            {lightPwrOn ? 'On' : 'Off'}
                                        </span>
                                    </button>
                                </div>
                                <h5 style={lightAuto ? { color: '#52e000', fontSize: '16px' } : {}} >Auto</h5>
                            </div>
                        </div>
                        <div id="lightScheduleGanttChartContainer" className="chart-gantt">
                        </div>
                    </div>

                    {/* Disposal History */}
                    <div className="container log-container">
                        <h3>Disposal History</h3>
                        <div className="disposal-table">
                            <table className="table-header-disp">
                                <thead>
                                    <tr>
                                        <th>Type</th>
                                        <th>Amount</th>
                                        <th>Time</th>
                                        <th>Date</th>
                                    </tr>
                                </thead>
                            </table>
                            <table className="table-body-disp">
                                <tbody>
                                {logHistory.map((entry, index) => (
                                    <tr key={index}>
                                        <td>{entry.Type}</td>
                                        <td>{entry.Amount}mL</td>
                                        <td>{entry.time}</td>
                                        <td>{entry.date}</td>
                                    </tr>
                                ))}
                                </tbody>
                            </table>
                        </div>
                    </div>

                    {/*Daily Avg Chart*/}
                    <div className="container avg-chart-container">
                        <h3>Average Chart</h3>
                        <div className="avg-chart" ref={avgChartContainerRef}>
                            <canvas ref={avgChartRef} id="avgChart"></canvas>
                        </div>
                    </div>

                    {/*Daily Samples Chart*/}
                    <div className="container daily-chart-container">
                        <h3>Daily Chart</h3>
                        <div className="select-container">
                            <label htmlFor="day-selector">Select Day: </label>
                            <select className="custom-select"
                                    id="day-selector"
                                    value={selectedDay}
                                    onChange={(e) => setSelectedDay(e.target.value)}
                            >
                                 {sortedUniqueDates.map((uniqueDate) => (
                                    <option key={uniqueDate} value={uniqueDate}>
                                        {moment(uniqueDate).format('MMM D, YYYY')}
                                    </option>
                                ))}
                            </select>
                        </div>
                        <div className="daily-chart">
                            <canvas ref={dailyChartRef} id="dailyChart"></canvas>
                        </div>
                    </div>
                </main>
            </div>
        </div>
    );
};

export default Overview;
