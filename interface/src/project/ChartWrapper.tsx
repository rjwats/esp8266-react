import { Box, styled } from "@material-ui/core";

const ChartWrapper = styled(Box)({
  '& canvas': {
    width: '100% !important'
  }
});

export default ChartWrapper;
