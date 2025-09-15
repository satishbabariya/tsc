
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

// Dynamic imports with paths
async function loadUserModule() {
    const userModule = await import('./modules/user');
    return userModule;
}

async function loadAdminModule() {
    const adminModule = await import('./modules/admin');
    return adminModule;
}

// Conditional imports
async function loadModule(moduleName: string) {
    switch (moduleName) {
        case 'user':
            return await import('./modules/user');
        case 'admin':
            return await import('./modules/admin');
        case 'guest':
            return await import('./modules/guest');
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

// Test dynamic imports
loadUserModule().then(module => {
    console.log("User module loaded:", module);
});

loadAdminModule().then(module => {
    console.log("Admin module loaded:", module);
});

// Test conditional imports
loadModule('user').then(module => {
    console.log("User module loaded conditionally:", module);
});

console.log("Module Resolution test completed successfully!");
