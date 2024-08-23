import { auth, database } from "../../firebaseConfig";
import { ref, onValue, set, get } from "firebase/database";

export const handleTogglePump = async (pump, systemName) => {
    const currentUser = auth.currentUser;

    if (currentUser) {
        const pumpRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/DosingPumps/${pump}`);
        const snapshot = await get(pumpRef);
        const currentStatus = snapshot.val();
        await set(pumpRef, !currentStatus);
    }
};

export const fetchDP1Status = (setDP1Status, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const DP1StatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/DosingPumps/DP1`);
        const unsubscribe = onValue(DP1StatusRef, (snapshot) => {
            const value = snapshot.val();
            setDP1Status(value !== null ? value : false);
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};

export const fetchDP2Status = (setDP2Status, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const DP2StatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/DosingPumps/DP2`);
        const unsubscribe = onValue(DP2StatusRef, (snapshot) => {
            const value = snapshot.val();
            setDP2Status(value !== null ? value : false);
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};


export const fetchDP3Status = (setDP3Status, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const DP3StatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/DosingPumps/DP3`);
        const unsubscribe = onValue(DP3StatusRef, (snapshot) => {
            const value = snapshot.val();
            setDP3Status(value !== null ? value : false);
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};

export const fetchDP4Status = (setDP4Status, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const DP4StatusRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/DosingPumps/DP4`);
        const unsubscribe = onValue(DP4StatusRef, (snapshot) => {
            const value = snapshot.val();
            setDP4Status(value !== null ? value : false);
        });

        // Return a cleanup function
        return () => unsubscribe();
    } else {
        // If no user, return a no-op cleanup function
        return () => {};
    }
};

