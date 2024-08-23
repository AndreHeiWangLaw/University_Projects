import { auth, database } from "../../firebaseConfig";
import { ref, onValue, set, get } from "firebase/database";
import Highcharts from 'highcharts';
import HighchartsGantt from 'highcharts/modules/gantt';

HighchartsGantt(Highcharts);

export const fetchLightTimes = (setLightStart, setLightEnd, setLightScheduleAuto, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const lightStartRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/lightON`);
        const lightEndRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/lightOFF`);
        const lightScheduleAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/LightAuto`);

        const unsubscribeStart = onValue(lightStartRef, (snapshot) => {
            const startTimeValue = snapshot.val();
            if (startTimeValue) setLightStart(startTimeValue);
        });

        const unsubscribeEnd = onValue(lightEndRef, (snapshot) => {
            const endTimeValue = snapshot.val();
            if (endTimeValue) setLightEnd(endTimeValue);
        });

        const unsubscribeAuto = onValue(lightScheduleAutoRef, (snapshot) => {
            const scheduleAutoValue = snapshot.val();
            setLightScheduleAuto(scheduleAutoValue);
        });

        // Return a cleanup function
        return () => {
            unsubscribeStart();
            unsubscribeEnd();
            unsubscribeAuto();
        };
    }
};

export const updateLightStart = (newTime, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const lightStartRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/lightON`);
        set(lightStartRef, newTime);
    }
};

export const updateLightEnd = (newTime, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const lightEndRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/lightOFF`);
        set(lightEndRef, newTime);
    }
};

export const toggleLightScheduleAuto = (currentState, setLightScheduleAuto, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const lightScheduleAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/LightAuto`);
        const newState = !currentState;
        set(lightScheduleAutoRef, newState);
        setLightScheduleAuto(newState);
    }
};

function timeToMinutes(time) {
    if (!time) return 0;
    const [hours, minutes] = time.split(':');
    return parseInt(hours, 10) * 60 + parseInt(minutes, 10);
}

export const calculateDayNightDurations = (lightStart, lightEnd) => {
    const totalMinutesInDay = 24 * 60;

    const startMinutes = timeToMinutes(lightStart);
    const endMinutes = timeToMinutes(lightEnd);

    let dayDuration, nightDuration;
    if (startMinutes < endMinutes) {
        // Day starts in the morning and ends in the evening
        dayDuration = endMinutes - startMinutes;
        nightDuration = totalMinutesInDay - dayDuration;
    } else {
        // Day starts in the evening and ends in the morning (overnight)
        nightDuration = startMinutes - endMinutes;
        dayDuration = totalMinutesInDay - nightDuration;
    }

    return {
        dayDuration,   // Duration of daylight in minutes
        nightDuration  // Duration of nighttime in minutes
    };
};

// Toggle light power
export const handleToggleLight = async (systemName) => {
    const currentUser = auth.currentUser;

    if (currentUser) {
        const lightControlRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/LightPower`);
        const snapshot = await get(lightControlRef);
        const currentStatus = snapshot.val();
        await set(lightControlRef, !currentStatus); // Toggle the current status
    }
};

// Fetch current light power status
export const fetchLigthPowerStatus = (setLightPowerOn, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const lightPowerStatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LightControl/LightPower`);
        const unsubscribe = onValue(lightPowerStatusRef, (snapshot) => {
            const value = snapshot.val();
            setLightPowerOn(value !== null ? value : false); // Update the state based on the value from the database
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};

export const createLightScheduleGanttChart = (containerId, lightON, lightOFF, isDarkMode) => {
    const currentTime = new Date();
    const currentHours = currentTime.getHours();
    const currentMinutes = currentTime.getMinutes();
    const currentTimeInMs = Date.UTC(2024, 0, 1, currentHours, currentMinutes);

    const padZero = (number) => {
        return number < 10 ? `0${number}` : number.toString();
    };

    const formattedCurrentHours = padZero(currentHours);
    const formattedCurrentMinutes = padZero(currentMinutes);

    const textColor = isDarkMode ? 'white' : 'grey';
    const plotLineColor = isDarkMode ? 'white' : 'black';

    Highcharts.ganttChart(containerId, {
        chart: {
            backgroundColor: null,
            type: 'gantt',
            marginTop: 0, // Space at the top
            marginRight: 0, // Space on the right
            marginBottom: 45, // Space at the bottom
            marginLeft: 0, // Space on the left
            spacing: [0, 0, 0, 0],
            style: {
                color: textColor, // Set text color for the chart
            },
        },
        title: {
            text: ''
        },
        xAxis: [{
            visible: true,
            type: 'datetime',
            plotLines: [{
                color: plotLineColor,
                width: 2,
                value: currentTimeInMs,
                zIndex: 5,
                label: {
                    text: `Curr. Time: ${formattedCurrentHours}:${formattedCurrentMinutes}`,
                    x: -55, 
                    y: 120, 
                    rotation : 0,
                    style: {
                        color: textColor,
                        fontSize: 'smaller',
                    }
                }
            }],
            labels: {
                enabled: false
            },
            tickLength: 1,
            lineWidth: 1,
            minorGridLineWidth: 1,
            minorTickLength: 1,
        }],
        yAxis: {
            visible: true,
            gridLineWidth: 1,
            labels: {
                enabled: true,
                style: {
                    color: textColor, // Adjust axis labels color based on dark mode
                }
            }
        },
        series: [{
            name: 'Light Schedule',
            data: [{
                start: Date.UTC(2024, 0, 1, parseInt(lightON.split(':')[0]), parseInt(lightON.split(':')[1])),
                end: Date.UTC(2024, 0, 1, parseInt(lightOFF.split(':')[0]), parseInt(lightOFF.split(':')[1])),
                name: 'Day',
                color: '#00a9cf'
            }, {
                start: Date.UTC(2024, 0, 1, parseInt(lightOFF.split(':')[0]), parseInt(lightOFF.split(':')[1])),
                end: Date.UTC(2024, 0, 2, parseInt(lightON.split(':')[0]), parseInt(lightON.split(':')[1])),
                name: 'Night',
                color: '#024554'
            }],
            dataLabels: {
                enabled: false
            }
        }],
        tooltip: {
            formatter: function () {
                const start = Highcharts.dateFormat('%H:%M', this.point.options.start);
                const end = Highcharts.dateFormat('%H:%M', this.point.options.end);
                const seriesName = this.point.y === 0 ? 'Day' : 'Night';
                return `<b>${seriesName}</b><br/>Start: ${start}</b><br/>End: ${end}<br/> Current Time: ${formattedCurrentHours}:${formattedCurrentMinutes} `;
            }
        },
        credits: {
            enabled: false
        },
        exporting: {
            enabled: false
        },
        plotOptions: {
            series: {
                borderRadius: 5,
                pointPadding: 0.1,
                groupPadding: 0,
                borderColor: 'transparent', // Ensure no border color
                edgeColor: 'transparent', // Ensures no edge color
                borderWidth: 0 // Ensures no border width
            }
        },
        plotBackgroundImage: '',
        plotBorderColor: 'transparent',
        plotBorderWidth: 0,
        plotShadow: false
    });
};




