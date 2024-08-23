import { auth, database } from "../../firebaseConfig";
import { ref, onValue, set, get } from "firebase/database";
import { debounce } from "lodash";

export const fetchPhMinMax = async (setPhMin, setPhMax, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const phMinRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phMin`);
        const phMaxRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phMax`);

        onValue(phMinRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setPhMin(parseFloat(value).toFixed(1));
            }
        });

        onValue(phMaxRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setPhMax(parseFloat(value).toFixed(1));
            }
        });
    }
};

export const fetchPhValue = async (setPhValue, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const phSensorRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/LiveFeed/phValue`);
        onValue(phSensorRef, debounce((snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setPhValue(value.toFixed(2));
            }
        }, 1000));
    }
};

export const updatePhMinMax = async (phMin, phMax, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const phMinRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phMin`);
        const phMaxRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phMax`);

        const currentPhMinSnapshot = await get(phMinRef);
        const currentPhMaxSnapshot = await get(phMaxRef);

        const currentPhMin = parseFloat(currentPhMinSnapshot.val());
        const currentPhMax = parseFloat(currentPhMaxSnapshot.val());

        if (parseFloat(phMin) > currentPhMax || parseFloat(phMax) < currentPhMin) {
            console.error("Invalid pH range provided");
            return;
        }

        set(phMinRef, parseFloat(phMin));
        set(phMaxRef, parseFloat(phMax));
    }
};

export const togglePhAuto = (phAuto, setPhAuto, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const phAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phAuto`);
        set(phAutoRef, !phAuto).then(() => {
            setPhAuto(!phAuto);
        }).catch((error) => {
            console.error('Error updating Ph Auto:', error);
        });
    }
};

export const fetchPhAutoStatus = async (setPhAuto, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const phAutoRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/phBalance/phAuto`);
        onValue(phAutoRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) {
                setPhAuto(value);
            }
        });
    }
};
