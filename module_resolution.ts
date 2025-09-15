
// Module Resolution Test
// Test path-based module loading

// Relative imports
import { helper } from './helpers/helper';
import { utils } from '../utils/utils';
import { config } from '../../config/config';

// Absolute imports
import { api } from '/src/api/api';
import { database } from '/src/database/database';

// Package imports
import React from 'react';
import { useState, useEffect } from 'react';
import * as lodash from 'lodash';
import { debounce } from 'lodash/debounce';

// Scoped package imports
import { Button } from '@mui/material';
import { ThemeProvider } from '@mui/material/styles';

// Type-only imports
import type { ComponentProps } from 'react';
import type { User } from './types/user';

// Static imports with paths
import { UserModule } from './modules/user';
import { AdminModule } from './modules/admin';
import { GuestModule } from './modules/guest';

// Conditional module usage (static)
function useModule(moduleName: string) {
    switch (moduleName) {
        case 'user':
            return new UserModule();
        case 'admin':
            return new AdminModule();
        case 'guest':
            return new GuestModule();
        default:
            throw new Error(`Unknown module: ${moduleName}`);
    }
}

// Test module resolution
console.log("Helper:", helper);
console.log("Utils:", utils);
console.log("Config:", config);
console.log("API:", api);
console.log("Database:", database);

// Test React imports
const [state, setState] = useState(0);
useEffect(() => {
    console.log("Effect triggered");
}, []);

// Test lodash imports
const debouncedFunction = debounce(() => {
    console.log("Debounced function called");
}, 300);

// Test scoped package imports
const button = <Button>Click me</Button>;
const theme = <ThemeProvider theme={{}}>App</ThemeProvider>;

// Test static module usage
const userModule = useModule('user');
console.log("User module:", userModule);

const adminModule = useModule('admin');
console.log("Admin module:", adminModule);

// Test conditional module usage
const guestModule = useModule('guest');
console.log("Guest module:", guestModule);

console.log("Module Resolution test completed successfully!");
