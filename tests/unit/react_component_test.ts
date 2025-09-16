// React-like Component System Test
import React, {useCallback, useEffect, useState} from 'react';

// Generic component props
interface ComponentProps<T = any> {
    data: T;
    onUpdate?: (data: T) => void;
    children?: React.ReactNode;
}

// Generic component state
interface ComponentState<T> {
    data: T;
    loading: boolean;
    error: string | null;
}

// Generic hook
function useGenericState<T>(initialValue: T): [T, (value: T) => void] {
    const [state, setState] = useState<T>(initialValue);
    return [state, setState];
}

// Generic component
function GenericComponent<T>(props: ComponentProps<T>): JSX.Element {
    const [state, setState] = useGenericState<ComponentState<T>>({
        data: props.data,
        loading: false,
        error: null
    });

    const handleUpdate = useCallback((newData: T) => {
        setState(prev => ({...prev, data: newData}));
        props.onUpdate?.(newData);
    }, [props, setState]);

    useEffect(() => {
        setState(prev => ({...prev, loading: true}));
        // Simulate async operation
        setTimeout(() => {
            setState(prev => ({...prev, loading: false}));
        }, 1000);
    }, [setState]);

    return (
        <div>
            <h1>Generic
    Component < /h1>
    < p > Data
:
    {
        JSON.stringify(state.data)
    }
    </p>
    < p > Loading
:
    {
        state.loading ? 'Yes' : 'No'
    }
    </p>
    {
        state.error && <p>Error
    :
        {
            state.error
        }
        </p>}
        {
            props.children
        }
        </div>
    )
        ;
    }

// Usage
    const UserComponent = GenericComponent<{ id: number; name: string }>;
    const ProductComponent = GenericComponent<{ id: number; title: string; price: number }>;

    console.log("React-like component test completed");
