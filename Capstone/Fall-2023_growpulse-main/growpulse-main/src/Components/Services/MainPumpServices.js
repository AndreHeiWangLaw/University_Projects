import { auth, database } from "../../firebaseConfig";
import { ref, onValue, set, get } from "firebase/database";

export const handleToggleMainPump = async (systemName) => {
    const currentUser = auth.currentUser;

    if (currentUser) {
        const MainPumpRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/MainPump/PumpPower`);
        const snapshot = await get(MainPumpRef);
        const currentStatus = snapshot.val();
        await set(MainPumpRef, !currentStatus);
    }
};

export const fetchMainPumpStatus = (setMainPumpOn, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const MainPumpStatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/MainPump/PumpPower`);
        const unsubscribe = onValue(MainPumpStatusRef, (snapshot) => {
            const value = snapshot.val();
            setMainPumpOn(value !== null ? value : false);
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};


