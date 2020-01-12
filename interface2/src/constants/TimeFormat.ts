export const unixTimeToTimeAndDate = (unixTime: number) => new Date(unixTime * 1000).toLocaleString();
