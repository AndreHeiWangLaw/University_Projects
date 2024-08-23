// LandingPageController.js
import { useState, useRef } from "react";
import {features as featuresData} from "./LandingPageModel";

export const useLandingPageController = () => {
    const [startFeatureIndex, setStartFeatureIndex] = useState(0);
    const [isHeaderMinimized, setHeaderMinimized] = useState(false);
    const lastScrollTop = useRef(0);
    const [showScrollTop, setShowScrollTop] = useState(false);

    const scrollToSection = (ref) => {
        ref.current.scrollIntoView({ behavior: 'smooth', block: 'start' });
    };

    const scrollToFeatureDescription = async (index) => {
        const adjustedIndex = (startFeatureIndex + index) % featuresData.length;
        const allDescriptions = document.querySelectorAll('.feature-row');
        if (allDescriptions && allDescriptions[adjustedIndex]) {
            allDescriptions[adjustedIndex].scrollIntoView({ behavior: 'smooth', block: 'center' });
        }
    };

    const handleScroll = () => {
        let st = window.pageYOffset || document.documentElement.scrollTop;

        // If scrolling down and beyond 1000px
        if (st > lastScrollTop.current && st > 1000) {
            setHeaderMinimized(true);
        }
        // If scrolling up or if below 100px
        else {
            setHeaderMinimized(false);
        }

        // If scrolled beyond 400px
        if (st > 400) {
            setShowScrollTop(true);
        }
        // If scrolled to the top or less than 400px
        else if (st <= 400) {
            setShowScrollTop(false);
        }

        document.querySelectorAll('.feature-row').forEach((row) => {
            if (isElementInViewport(row)) {
                row.style.animation = `slideIn 0.5s forwards`;
            }
        });

        lastScrollTop.current = st <= 0 ? 0 : st; // For Mobile or negative scrolling
    };

    const isElementInViewport = (el) => {
        let rect = el.getBoundingClientRect();
        return (
            rect.top >= 0 &&
            rect.left >= 0 &&
            rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) &&
            rect.right <= (window.innerWidth || document.documentElement.clientWidth)
        );
    };

    return {
        startFeatureIndex,
        setStartFeatureIndex,
        isHeaderMinimized,
        lastScrollTop,
        showScrollTop,
        scrollToSection,
        scrollToFeatureDescription,
        handleScroll
    };
};
