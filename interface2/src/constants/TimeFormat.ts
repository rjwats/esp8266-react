export const unixTimeToTimeAndDate = (unixTime: number) => new Date(unixTime * 1000).toLocaleString();

export function humanReadableSeconds(duration: number) {
    const years = Math.floor(duration / 31536000),
        days = Math.floor((duration %= 31536000) / 86400),
        hours = Math.floor((duration %= 86400) / 3600),
        minutes = Math.floor((duration %= 3600) / 60),
        seconds = duration % 60;

    return (years ? years + "y " : "") +
        (years || days ? days + "d " : "") +
        (years || days || hours ? hours + "h " : "") +
        (years || days || hours || minutes ? minutes + "m " : "") +
        seconds + "s";
}