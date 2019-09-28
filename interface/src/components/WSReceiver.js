import React from 'react';
import Sockette from 'sockette';

export const wsReceiver = (wsUrl, FormComponent) => {

  return (
    class extends React.Component {

      componentDidMount() {
        this.onOpen = this.onOpen.bind(this);
        this.onClose = this.onClose.bind(this);
        this.onMessage = this.onMessage.bind(this);
        const ws = new Sockette(wsUrl, {
          onmessage: this.onMessage,
          onopen: this.onOpen,
          onclose: this.onClose
        });
        this.setState({
          ws,
          connected: false,
          data: null
        });
      }

      componentWillUnmount() {
        this.state.ws.close();
      }

      onOpen(event) {
        this.setState({connected: true});
      }

      onClose(event) {
        this.setState({connected: false});
      }

      onMessage(event) {
        const data = event.data;
        if (typeof data === 'string' || data instanceof String) {
          this.setState(
            {data: JSON.parse(data)}
          );
        }
      }

      render() {
        return <FormComponent
                  {...this.state}
                  {...this.props}
                />;
      }

    }
  );
}
