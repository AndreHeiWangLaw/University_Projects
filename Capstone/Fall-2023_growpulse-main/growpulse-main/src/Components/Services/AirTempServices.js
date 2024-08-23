import { auth, database } from "../../firebaseConfig";
import { ref, onValue } from "firebase/database";
import { debounce } from "lodash";

export const fetchAirTemperature = async (setAirTemperature, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const AirTemperatureRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LiveFeed/AirTemperature`);
        onValue(AirTemperatureRef, debounce((snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setAirTemperature(value.toFixed(1));
            }
        }, 1000));
    }
};
