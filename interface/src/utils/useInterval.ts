import { useEffect, useRef } from 'react';
import { isUndefined } from 'lodash';

function useInterval(callback: () => void, delay?: number) {
  const savedCallback = useRef(callback);

  // Remember the latest callback if it changes
  useEffect(() => {
    savedCallback.current = callback;
  }, [callback]);

  // Manage the interval if it changes
  useEffect(() => {
    if (isUndefined(delay)) {
      return;
    }
    const id = setInterval(() => savedCallback.current(), delay);
    return () => clearInterval(id);
  }, [delay]);
}

export default useInterval;
