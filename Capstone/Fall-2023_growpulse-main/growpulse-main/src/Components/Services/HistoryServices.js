import { auth, firestore } from "../../firebaseConfig";
import { doc, getDoc, updateDoc } from "firebase/firestore";

const updateTotalAmounts = async (logHistoryDocRef, totals) => {
    try {
        await updateDoc(logHistoryDocRef, totals);
    } catch (error) {
        console.error("Error updating totals: ", error);
    }
};

export const fetchLogHistory = async (setLogHistory, systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const logHistoryDocRef = doc(firestore, `Registered Users/${currentUser.uid}/${systemName}/DispenseHistory`);
        const docSnapshot = await getDoc(logHistoryDocRef);
        if (docSnapshot.exists()) {
            const data = docSnapshot.data();
            let formattedData = [];
            let totals = { totalUp: 0, totalDown: 0, totalA: 0, totalB: 0 };

            for (const [date, timeEntries] of Object.entries(data)) {
                for (const [time, logData] of Object.entries(timeEntries)) {
                    for (const [type, amount] of Object.entries(logData)) {
                        formattedData.push({
                            rawDate: date,
                            date: formatDate(date),
                            time,
                            Type: typeDisplayMapping[type] || type,
                            Amount: amount
                        });

                        if (type.includes('Up')) {
                            totals.totalUp += amount;
                        } else if (type.includes('Down')) {
                            totals.totalDown += amount;
                        } else if (type.includes('A')) {
                            totals.totalA += amount;
                        } else if (type.includes('B')) {
                            totals.totalB += amount;
                        }

                    }
                }
            }

            // Sort the formattedData array by date and time in descending order
            formattedData.sort((a, b) => {
                const aDateTime = new Date(`${a.rawDate}T${a.time}`);
                const bDateTime = new Date(`${b.rawDate}T${b.time}`);
                return bDateTime - aDateTime;
            });

            // Remove the rawDate property from each object
            formattedData = formattedData.map(({ rawDate, ...item }) => item);

            setLogHistory(formattedData);
            await updateTotalAmounts(logHistoryDocRef, totals);
        } else {
            setLogHistory([]);
        }
    }
};


const formatDate = (date) => {
    // eslint-disable-next-line
    const [year, month, day] = date.split("-");
    return `${day}/${month}`;
};

const typeDisplayMapping = {
    dispensedVolumeUp: "Manual pH Up",
    dispensedVolumeUpAuto: "Automatic pH Up",
    dispensedVolumeDown: "Manual pH Down",
    dispensedVolumeDownAuto: "Automatic pH Down",
    dispensedVolumeA_Init: "Initial Dose Sol. A",
    dispensedVolumeB_Init: "Initial Dose Sol. B",
    dispensedVolumeA: "Manual Dose Sol. A",
    dispensedVolumeB: "Manual Dose Sol. B"
};
