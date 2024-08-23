import { auth, database } from "../../firebaseConfig";
import { ref, onValue, set } from "firebase/database";
import { debounce } from "lodash";

export const fetchHumidityData = (setHumidityLevel, setScheduleAutomationHumidity, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const humidityLevelRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/humidityLevel`);
        const scheduleAutomationHumidityRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/automationHumidity`);

        onValue(humidityLevelRef, (snapshot) => {
            const humidityValue = snapshot.val();
            if (humidityValue) setHumidityLevel(humidityValue);
        });

        onValue(scheduleAutomationHumidityRef, (snapshot) => {
            const automationValue = snapshot.val();
            setScheduleAutomationHumidity(automationValue);
        });

        return () => {
            // Unsubscribe from changes
            humidityLevelRef.off();
            scheduleAutomationHumidityRef.off();
        };
    }
};

export const updateHumidityLevel = (newLevel, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const humidityLevelRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/humidityLevel`);
        set(humidityLevelRef, newLevel);
    }
};

export const toggleAutomationHumidity = (currentState, setScheduleAutomationHumidity, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const scheduleAutomationHumidityRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/automationHumidity`);

        // Setting the opposite value to the current state
        const newState = !currentState;
        set(scheduleAutomationHumidityRef, newState);
        setScheduleAutomationHumidity(newState);
    }
};

export const fetchHumidity = async (setAirHumidity, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const AirHumidityRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LiveFeed/Humidity`);
        onValue(AirHumidityRef, debounce((snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setAirHumidity(parseFloat(value));
            }
        }, 1000));
    }
};

export const fetchHumidityAutoStatus = async (setHumidityAuto, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const HumidityAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/HumidityControl/automationHumidity`);
        onValue(HumidityAutoRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setHumidityAuto(value);
            }
        });
    }
};
