import { auth } from "../../firebaseConfig";
import { ref, getDownloadURL } from "firebase/storage";
import { storage } from '../../firebaseConfig';

export const fetchImage = async (systemName) => {
    const currentUser = auth.currentUser;
    if (currentUser) {
        const storageRef = ref(storage, `Registered Users/${currentUser.uid}/${systemName}/placeHolder.jpeg`);
        try {
            const url = await getDownloadURL(storageRef);
            return url;
        } catch (error) {
            console.error("Error fetching image:", error);
            return null;
        }
    }
};