import React, { useState, useEffect } from 'react';
import './AlertsSettingsModal.css';
import {
    fetchPhMin, fetchPhMax, updatePhValues,
    fetchTdsMin, updateTdsMin,
    fetchWaterTempMin, fetchWaterTempMax, updateWaterTempMin, updateWaterTempMax,
    fetchAirTempMin, fetchAirTempMax, updateAirTempMin, updateAirTempMax,
    fetchHumidityOffset, updateHumidityOffset,
} from '../Services/AlertsServices';

const AlertsSettingsModal = ({ systemName, onClose, isDarkMode }) => {
    const formatValue = (value) => {
        const numericValue = parseFloat(value);
        return isNaN(numericValue) ? '0.00' : numericValue.toFixed(2);
    };

    const [phMin, setPhMin] = useState('0.00');
    const [phMax, setPhMax] = useState('0.00');
    const [tdsMin, setTdsMin] = useState('0.00');
    const [waterTempMin, setWaterTempMin] = useState('0.00');
    const [waterTempMax, setWaterTempMax] = useState('0.00');
    const [airTempMin, setAirTempMin] = useState('0.00');
    const [airTempMax, setAirTempMax] = useState('0.00');
    const [humidityOffset, setHumidityOffset] = useState('0.00');

    useEffect(() => {
        fetchPhMin(setPhMin, systemName);
        fetchPhMax(setPhMax, systemName);
        fetchTdsMin(setTdsMin, systemName);
        fetchWaterTempMin(setWaterTempMin, systemName);
        fetchWaterTempMax(setWaterTempMax, systemName);
        fetchAirTempMin(setAirTempMin, systemName);
        fetchAirTempMax(setAirTempMax, systemName);
        fetchHumidityOffset(setHumidityOffset, systemName);
    }, [systemName]);

    const handleChange = (currentValue, operation, setValue, limitMin, limitMax) => {
        let newValue = parseFloat(currentValue);
    
        if (isNaN(newValue)) {
            newValue = limitMin;
        } else {
            if (operation === 'increment') {
                newValue = newValue + 1;
                if (newValue > limitMax) {
                    newValue = limitMax;
                }
            } else if (operation === 'decrement') {
                newValue = newValue - 1;
                if (newValue < limitMin) {
                    newValue = limitMin;
                }
            }
        }
        newValue = Math.min(Math.max(newValue, limitMin), limitMax);
        setValue(formatValue(newValue));
    };

    const handleSliderChange = (e, setValue) => {
        setValue(formatValue(e.target.value));
    };

    const handleOutsideClick = (e) => {
        if (e.target.className === "alerts-modal-container") {
            onClose();
        }
    };

    const saveSettings = async () => {
        try {
            // Validate and convert state values to numbers
            const validatedPhMin = parseFloat(phMin);
            const validatedPhMax = parseFloat(phMax);
            const validatedTdsMin = parseFloat(tdsMin);
            const validatedWaterTempMin = parseFloat(waterTempMin);
            const validatedWaterTempMax = parseFloat(waterTempMax);
            const validatedAirTempMin = parseFloat(airTempMin);
            const validatedAirTempMax = parseFloat(airTempMax);
            const validatedHumidityOffset = parseFloat(humidityOffset);
    
            // Check for invalid numbers (NaN) and out-of-range values
            if (isNaN(validatedPhMin) || validatedPhMin < 0 || validatedPhMin > validatedPhMax) {
                throw new Error("Invalid pH minimum value");
            }
            if (isNaN(validatedPhMax) || validatedPhMax > 14 || validatedPhMax < validatedPhMin) {
                throw new Error("Invalid pH maximum value");
            }
            // Add similar validation checks for other parameters (TDS, Water Temp, etc.)
    
            // Update settings using the validated and converted values
            await Promise.all([
                updatePhValues(validatedPhMin, validatedPhMax, systemName),
                updateTdsMin(validatedTdsMin, systemName),
                updateWaterTempMin(validatedWaterTempMin, systemName),
                updateWaterTempMax(validatedWaterTempMax, systemName),
                updateAirTempMin(validatedAirTempMin, systemName),
                updateAirTempMax(validatedAirTempMax, systemName),
                updateHumidityOffset(validatedHumidityOffset, systemName),
                // Include other update functions as needed
            ]);
    
            console.log('Settings updated successfully');
            onClose(); // Close the modal or perform other UI updates as necessary
        } catch (error) {
            console.error('Error updating settings:', error);
            // Handle the error in the UI, such as showing an error message to the user
        }
    };    

    return (
        <div className={`alerts-modal-container ${isDarkMode ? 'dark-mode' : ''}`} onClick={handleOutsideClick}>
            <div className={`alerts-modal ${isDarkMode ? 'dark-mode' : ''}`}>
                <div className='modal-header'>
                    <h2>Alerts Settings for {systemName}</h2>
                    <button className="close-button" onClick={onClose}>X</button>
                </div>
                <table>
                    <tbody>
                    <tr>
                        <h4>pH:</h4>
                        <td>
                            <span>Min:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(phMin, 'decrement', setPhMin, 0, phMax)}> - </button>
                                <input className='input-text' type="text" value={phMin} onChange={(e) => setPhMin(e.target.value)} readOnly/>
                                <button onClick={() => handleChange(phMin, 'increment', setPhMin, 0, phMax)}> + </button>
                            </div>
                            <span>Max:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(phMax, 'decrement', setPhMax, phMin, 14)}> - </button>
                                <input className='input-text' type="text" value={phMax} onChange={(e) => setPhMax(e.target.value)}  readOnly/>
                                <button onClick={() => handleChange(phMax, 'increment', setPhMax, phMin, 14)}> + </button>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <h4>TDS:</h4>
                        <td>
                            <span className='slider-text'>Min:</span> 
                            <input 
                                type="range" 
                                min="0" 
                                max="2000" 
                                value={tdsMin} 
                                step="10" 
                                onChange={(e) => handleSliderChange(e, setTdsMin)} 
                            />
                            <h5 className='slider-value'>{tdsMin} ppm</h5>
                        </td>
                    </tr>
                    <tr>
                        <h4>Water Temperature:</h4>
                        <td>
                            <span>Min:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(waterTempMin, 'decrement', setWaterTempMin, 0, waterTempMax)}> - </button>
                                <input className='input-text' type="text" value={waterTempMin} onChange={(e) => setWaterTempMin(e.target.value)} readOnly/>
                                <button onClick={() => handleChange(waterTempMin, 'increment', setWaterTempMin, 0, waterTempMax)}> + </button>
                            </div>
                            <h6>°C</h6>
                            <span>Max:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(waterTempMax, 'decrement', setWaterTempMax, waterTempMin, 24)}> - </button>
                                <input className='input-text' type="text" value={waterTempMax} onChange={(e) => setWaterTempMax(e.target.value)} readOnly/>
                                <button onClick={() => handleChange(waterTempMax, 'increment', setWaterTempMax, waterTempMin, 24)}> + </button>
                            </div>
                            <h6>°C</h6>
                        </td>
                    </tr>
                    <tr>
                        <h4>Humidity:</h4>
                        <td>
                            <span className='slider-text'>Offset:</span>
                            <input
                                type="range"
                                min="5"
                                max="100"
                                value={humidityOffset}
                                onChange={(e) => handleSliderChange(e, setHumidityOffset)}
                            />
                            <h5 className='slider-value'>{humidityOffset} %</h5>
                        </td>
                    </tr>
                    <tr>
                        <h4>Air Temperature:</h4>
                        <td>
                            <span>Min:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(airTempMin, 'decrement', setAirTempMin, 0, airTempMax)}> - </button>
                                <input className='input-text' type="text" value={airTempMin} onChange={(e) => setAirTempMin(e.target.value)} readOnly/>
                                <button onClick={() => handleChange(airTempMin, 'increment', setAirTempMin, 0, airTempMax)}> + </button>
                            </div>
                            <h6>°C</h6>
                            <span>Max:</span>
                            <div className="input-container">
                                <button onClick={() => handleChange(airTempMax, 'decrement', setAirTempMax, airTempMin, 30)}> - </button>
                                <input className='input-text' type="text" value={airTempMax} onChange={(e) => setAirTempMax(e.target.value)} readOnly/>
                                <button onClick={() => handleChange(airTempMax, 'increment', setAirTempMax, airTempMin, 30)}> + </button>
                            </div>
                            <h6>°C</h6>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <div className='save-button-div'>
                    <button className="save-button" onClick={saveSettings}>Save</button>
                </div>
            </div>
        </div>
    );
};

export default AlertsSettingsModal;
