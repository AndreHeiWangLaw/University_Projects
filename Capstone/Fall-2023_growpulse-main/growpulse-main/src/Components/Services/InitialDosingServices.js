import { getAuth } from "firebase/auth";
import { ref, onValue, set } from "firebase/database";
import { database } from "../../firebaseConfig";

export const fetchInitialDosingValues = async (setStartInitialDosing, setSolutionA_Dose, setSolutionB_Dose, setSystemVolume, systemName) => {
    const currentUser = getAuth().currentUser;
    if (currentUser) {
        const startInitialDosingRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/startInitialDosing`);
        const solutionA_DoseRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/A`);
        const solutionB_DoseRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/B`);
        const systemVolumeRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/systemVolume`);

        onValue(startInitialDosingRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) setStartInitialDosing(value);
        });

        onValue(solutionA_DoseRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) setSolutionA_Dose(parseFloat(value).toFixed(1));
        });

        onValue(solutionB_DoseRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) setSolutionB_Dose(parseFloat(value).toFixed(1));
        });

        onValue(systemVolumeRef, (snapshot) => {
            const value = snapshot.val();
            if (value !== null) setSystemVolume(parseFloat(value).toFixed(1));
        });
    }
};

export const updateInitialDosingValues = async (solutionA_Dose, solutionB_Dose, systemVolume, systemName) => {
    const currentUser = getAuth().currentUser;
    if (currentUser) {
        const solutionA_DoseRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/A`);
        const solutionB_DoseRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/B`);
        const systemVolumeRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/Amounts/systemVolume`);

        await set(solutionA_DoseRef, parseFloat(solutionA_Dose));
        await set(solutionB_DoseRef, parseFloat(solutionB_Dose));
        await set(systemVolumeRef, parseFloat(systemVolume));
    }
};

export const toggleStartInitialDosing = async (startInitialDosing, setStartInitialDosing, systemName) => {
    const currentUser = getAuth().currentUser;
    if (currentUser) {
        const startInitialDosingRef = ref(database, `Registered Users/${currentUser.uid}/${systemName}/InitialDose/startInitialDosing`);
        set(startInitialDosingRef, !startInitialDosing).then(() => {
            setStartInitialDosing(!startInitialDosing);
        }).catch((error) => {
            console.error('Error updating Start Initial Dosing:', error);
        });
    }
};
