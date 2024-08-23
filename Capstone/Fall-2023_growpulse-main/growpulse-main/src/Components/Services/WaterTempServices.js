import { auth, database } from "../../firebaseConfig";
import { ref, onValue } from "firebase/database";
import { debounce } from "lodash";

export const fetchWaterTempValue = async (setWaterTempValue, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const waterTempRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LiveFeed/WaterTemperature`);
        onValue(waterTempRef, debounce((snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setWaterTempValue(value.toFixed(1));
            }
        }, 1000));
    }
};
