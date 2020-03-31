import React from 'react';
import Sockette from 'sockette';
import throttle from 'lodash/throttle';

import { withSnackbar, WithSnackbarProps } from 'notistack';

import { RestControllerProps, extractEventValue } from '.';

interface SocketControllerState<D> {
  ws: Sockette;
  loading: boolean;
  data?: D;
}

export function socketController<D, P extends RestControllerProps<D>>(wsUrl: string, wsThrottle: number, SocketController: React.ComponentType<P & RestControllerProps<D>>) {
  return withSnackbar(
    class extends React.Component<Omit<P, keyof RestControllerProps<D>> & WithSnackbarProps, SocketControllerState<D>> {

      constructor(props: Omit<P, keyof RestControllerProps<D>> & WithSnackbarProps) {
        super(props);
        this.state = {
          ws: new Sockette(wsUrl, {
            onmessage: this.onMessage,
            onopen: this.onOpen,
            onclose: this.onClose,
          }),
          data: undefined,
          loading: true
        }
      }

      onMessage = (event: MessageEvent) => {
        const rawData = event.data;
        if (typeof rawData === 'string' || rawData instanceof String) {
          var data = JSON.parse(rawData as string) as D;
          this.setState({ data });
        }
      }

      onOpen = () => {
        this.setState({ loading: false });
      }

      onClose = () => {
        this.setState({ loading: true, data: undefined });
      }

      setData = (data: D) => {
        this.setState({ data });
      }

      saveData = throttle(() => {
        const { ws, loading, data } = this.state;
        if (!loading) {
          ws.json(data);
        }
      }, wsThrottle);


      loadData = () => {
        this.state.ws.reconnect()
      }

      handleValueChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
        const data = { ...this.state.data!, [name]: extractEventValue(event) };
        this.setState({ data });
      }

      handleSliderChange = (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => {
        const data = { ...this.state.data!, [name]: value };
        this.setState({ data });
      };

      render() {
        return <SocketController
          handleValueChange={this.handleValueChange}
          handleSliderChange={this.handleSliderChange}
          setData={this.setData}
          saveData={this.saveData}
          loadData={this.loadData}
          {...this.state}
          {...this.props as P}
        />;
      }

    });
}
