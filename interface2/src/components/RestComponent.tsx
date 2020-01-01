import React from 'react';
import { withSnackbar, WithSnackbarProps } from 'notistack';
import { redirectingAuthorizedFetch } from '../authentication/Authentication';

export interface RestComponentProps<D> {
  handleValueChange: (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => void;
  handleCheckboxChange: (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>, checked: boolean) => void;
  handleSliderChange: (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => void;

  setData: (data: D) => void;
  saveData: () => void;
  loadData: () => void;

  data?: D;
  fetched: boolean;
  errorMessage?: string;
}

interface RestComponentState<D> {
  data?: D;
  fetched: boolean;
  errorMessage?: string;
}

export function restComponent<D>(endpointUrl: string, FormComponent: React.ComponentType<RestComponentProps<D>>): React.ComponentType {
  return withSnackbar(class extends React.Component<WithSnackbarProps, RestComponentState<D>> {

    constructor(props: WithSnackbarProps) {
      super(props);
      this.state = {
        data: undefined,
        fetched: false,
        errorMessage: undefined
      };
    }

    setData = (data: D) => {
      this.setState({
        data,
        fetched: true,
        errorMessage: undefined
      });
    }

    loadData = () => {
      this.setState({
        data: undefined,
        fetched: false,
        errorMessage: undefined
      });
      redirectingAuthorizedFetch(endpointUrl).then(response => {
        if (response.status === 200) {
          return response.json();
        }
        throw Error("Invalid status code: " + response.status);
      }).then(json => {
        this.setState({ data: json, fetched: true })
      }).catch(error => {
        const errorMessage = error.message || "Unknown error";
        this.props.enqueueSnackbar("Problem fetching: " + errorMessage, {
          variant: 'error',
        });
        this.setState({ data: undefined, fetched: true, errorMessage });
      });
    }

    saveData = () => {
      this.setState({ fetched: false });
      redirectingAuthorizedFetch(endpointUrl, {
        method: 'POST',
        body: JSON.stringify(this.state.data),
        headers: {
          'Content-Type': 'application/json'
        }
      }).then(response => {
        if (response.status === 200) {
          return response.json();
        }
        throw Error("Invalid status code: " + response.status);
      }).then(json => {
        this.props.enqueueSnackbar("Changes successfully applied.", {
          variant: 'success',
        });
        this.setState({ data: json, fetched: true });
      }).catch(error => {
        const errorMessage = error.message || "Unknown error";
        this.props.enqueueSnackbar("Problem saving: " + errorMessage, {
          variant: 'error',
        });
        this.setState({ data: undefined, fetched: true, errorMessage });
      });
    }

    handleValueChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
      const data = { ...this.state.data!, [name]: event.target.value };
      this.setState({ data });
    }

    handleCheckboxChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
      const data = { ...this.state.data!, [name]: event.target.checked };
      this.setState({ data });
    }

    handleSliderChange = (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => {
      const data = { ...this.state.data!, [name]: value };
      this.setState({ data });
    };

    render() {
      return <FormComponent
        handleValueChange={this.handleValueChange}
        handleCheckboxChange={this.handleCheckboxChange}
        handleSliderChange={this.handleSliderChange}
        setData={this.setData}
        saveData={this.saveData}
        loadData={this.loadData}
        {...this.state}
      />;
    }

  });

}
